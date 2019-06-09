#include <iostream>
#include <memory>
#include <string>

#include "command_factory.h"
#include "interpreter.h"
#include "executer.h"
#include "execution_logger.h"

using namespace std;
using namespace griha;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: bulk <block_size>" << endl;
        return -1;
    }
    
    const size_t block_size = stoul(argv[1]);

    Interpreter::Context context = {
        make_shared<ExecutionLogger>(
            make_shared<StreamExecuter>(cout)
        ),
        make_shared<SimpleCommandFactory>(),
        block_size
    };

    Interpreter interpreter(context);
    interpreter.run(cin);
    return 0;
}