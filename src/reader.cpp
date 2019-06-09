#include "reader.h"

#include <cassert>
#include <algorithm>
#include <string>
#include <string_view>

#include "command_factory.h"

namespace griha {

namespace {

using namespace std::string_view_literals;

constexpr auto c_explicit_block_begin   = "{"sv;
constexpr auto c_explicit_block_end     = "}"sv;

bool is_block_begin(std::string_view line) {
    using namespace std;
    return line == c_explicit_block_begin;
}

bool is_block_end(std::string_view line) {
    using namespace std;
    return line == c_explicit_block_end;
}

} // unnamed namespace

struct ReaderState : std::enable_shared_from_this<ReaderState> {
    virtual ~ReaderState() {}
    virtual bool process(std::istream& line) = 0;
};
using ReaderStatePtr = std::shared_ptr<ReaderState>;

struct ReaderImpl {

    inline ReaderImpl(CommandFactoryPtr cf, size_t bsize)
        : state(nullptr)
        , block_size(bsize)
        , command_factory(cf) {}

    ReaderStatePtr state;
    const size_t block_size;

    std::vector<ReaderSubscriberPtr> subscribers;

    CommandFactoryPtr command_factory;
    CommandContainer commands;

    void add_command(std::string line);

    void notify_block_begin();
    void notify_block_end();
};

struct InitialState : ReaderState {
    inline explicit InitialState(ReaderImpl& r_impl)
        : reader_impl(r_impl) {}

    bool process(std::istream& line) override;

    ReaderImpl& reader_impl;
};

struct BlockState : ReaderState {
    inline BlockState(ReaderImpl& r_impl, ReaderStatePtr l_state) 
        : reader_impl(r_impl)
        , last_state(l_state) {}

    bool process(std::istream& line) override;

    ReaderImpl& reader_impl;
    ReaderStatePtr last_state;
};

void ReaderImpl::add_command(std::string line) {
    commands.push_back(command_factory->create_command(std::move(line)));
    if (commands.size() == 1)
        notify_block_begin();
}

void ReaderImpl::notify_block_begin() {
    assert(!commands.empty());
    for (auto& subscriber : subscribers)
        subscriber->on_block_begin(commands.front());
}

void ReaderImpl::notify_block_end() {
    assert(!commands.empty());
    for (auto& subscriber : subscribers)
        subscriber->on_block_end(commands);
    commands.clear();
}

bool InitialState::process(std::istream& input) {
    using namespace std;

    string line;
    if (!getline(input, line)) {
        if (!reader_impl.commands.empty())
            // in initial state the end of the stream triggers end of block
            reader_impl.notify_block_end(); 
        
        return false; // end of file or another error
    }
    
    auto this_ptr = shared_from_this(); // protect against unexpected deletion
    if (is_block_begin(line)) {
        // in initial state start of explicit block triggers end of block
        if (!reader_impl.commands.empty())
            reader_impl.notify_block_end(); 
        reader_impl.state = std::make_shared<BlockState>(reader_impl, nullptr);
    } else {
        reader_impl.add_command(std::move(line));
        // fixed block size has been reached
        if (reader_impl.commands.size() == reader_impl.block_size)
           reader_impl.notify_block_end();
    }

    return true;
}

bool BlockState::process(std::istream& input) {
    using namespace std;

    string line;
    if (!getline(input, line)) {
        return false; // end of file or another error
    }

    auto this_ptr = shared_from_this(); // protect against unexpected deletion
    if (is_block_begin(line)) {
        // nested explicit blocks are ignored but correction of syntax is required
        reader_impl.state = std::make_shared<BlockState>(reader_impl, reader_impl.state);
    } else if (is_block_end(line)) {
        // restore last state
        if (last_state == nullptr) {
            // explicit block has been ended
            reader_impl.notify_block_end();
            reader_impl.state = std::make_shared<InitialState>(reader_impl);
        } else {
            // nested explicit block has been ended
            reader_impl.state = last_state;
        }
    } else {
        reader_impl.add_command(std::move(line));
    }

    return true;
}

Reader::Reader(CommandFactoryPtr command_factory, size_t block_size)
    : priv_(std::make_unique<ReaderImpl>(command_factory, block_size)) {}

Reader::~Reader() = default;
Reader::Reader(Reader&&) = default;
Reader& Reader::operator= (Reader&&) = default;

void Reader::subscribe(ReaderSubscriberPtr subscriber) {
    auto& subscribers = priv_->subscribers;
    auto it = std::find(subscribers.begin(), subscribers.end(), subscriber);
    if (it == subscribers.end())
        subscribers.push_back(subscriber);
}

void Reader::run(std::istream& input) {
    priv_->state = std::make_shared<InitialState>(*priv_);

    while (priv_->state->process(input)) {
        // do nothing
    }
}

} // namespace griha