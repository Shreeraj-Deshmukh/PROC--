#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

// Order matters: composite patterns before their substrings
const std::vector<std::pair<std::string, std::string>> MAPPINGS = {
    // basesalary before base and salary
    {R"(\bbasesalary\b)",  "main"},

    // proc-prefixed keywords
    {R"(\bprocint\b)",     "int"},
    {R"(\bprocfloat\b)",   "float"},
    {R"(\bprocdouble\b)",  "double"},
    {R"(\bprocstring\b)",  "string"},
    {R"(\bprocvoid\b)",    "void"},
    {R"(\bprocauto\b)",    "auto"},
    {R"(\bprocprint\b)",   "cout"},
    {R"(\bprocin\b)",      "cin"},
    {R"(\bprocreturn\b)",  "return"},
    {R"(\bprocfor\b)",     "for"},
    {R"(\bprocloop\b)",    "while"},
    {R"(\bprocif\b)",      "if"},
    {R"(\bprocelse\b)",    "else"},
    {R"(\bprocase\b)",     "case"},
    {R"(\bprocbreak\b)",   "break"},
    {R"(\bproclife\b)",    "continue"},
    {R"(\bproclass\b)",    "class"},
    {R"(\bproctrue\b)",    "true"},
    {R"(\bprocfalse\b)",   "false"},
    {R"(\bprocvate\b)",    "private"},
    {R"(\bprocted\b)",     "protected"},

    // standalone keywords
    {R"(\bfire\b)",        "delete"},
    {R"(\bhire\b)",        "new"},
    {R"(\bpromote\b)",     "push_back"},
    {R"(\bzs\b)",          "switch"},
    {R"(\bnes\b)",         "bool"},
    {R"(\bvacant\b)",      "nullptr"},
    {R"(\bmemo\b)",        "struct"},
    {R"(\bdisclosed\b)",   "public"},
    {R"(\bdepartment\b)",  "namespace"},
    {R"(\bblueprint\b)",   "template"},
    {R"(\battempt\b)",     "try"},
    {R"(\bescalate\b)",    "catch"},
    {R"(\breport\b)",      "throw"},
    {R"(\broster\b)",      "vector"},

    // salary and base after basesalary
    {R"(\bsalary\b)",      "const"},
    {R"(\bbase\b)",        "static"},

    // preprocessor
    {R"(#onboard\b)",      "#include"},
};

// Keywords that open a block — never get a trailing semicolon
static const std::vector<std::string> CTRL = {
    "procfor", "procloop", "procif", "procelse", "zs", "attempt", "escalate"
};

static bool startsWith(const std::string& s, const std::string& prefix) {
    return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
}

static bool needsSemicolon(const std::string& line) {
    // trim trailing whitespace for inspection
    size_t end = line.find_last_not_of(" \t\r");
    if (end == std::string::npos) return false;
    std::string t = line.substr(0, end + 1);

    if (t.empty() || t[0] == '#' || t[0] == '/') return false;

    char last = t.back();

    // already terminated or structural
    if (last == ';' || last == '{' || last == '}' || last == ':' || last == ',')
        return false;

    if (last == ')') {
        for (const auto& kw : CTRL)
            if (startsWith(t, kw)) return false;
        // method call: has '.' → needs ;
        // return with call: starts with procreturn → needs ;
        if (t.find('.') != std::string::npos) return true;
        if (startsWith(t, "procreturn"))       return true;
        return false; // function definition or unknown — be safe
    }

    return std::isalnum((unsigned char)last) || last == '_' ||
           last == '"' || last == '\'' || last == ']';
}

std::string preprocess(const std::string& source) {
    std::istringstream stream(source);
    std::ostringstream out;
    std::string line;

    while (std::getline(stream, line)) {
        // procprint("msg") or procprint(var)  →  cout << expr << "\n";
        line = std::regex_replace(line,
            std::regex(R"(\bprocprint\((.+)\))"),
            R"(cout << $1 << "\n";)");

        // procin(var)  →  cin >> var;
        line = std::regex_replace(line,
            std::regex(R"(\bprocin\((\w+)\))"),
            R"(cin >> $1;)");

        // semicolon inference
        if (needsSemicolon(line)) {
            size_t end = line.find_last_not_of(" \t\r");
            line = line.substr(0, end + 1) + ";";
        }

        out << line << "\n";
    }

    return out.str();
}

std::string transpile(const std::string& source) {
    std::string result = source;
    for (const auto& m : MAPPINGS)
        result = std::regex_replace(result, std::regex(m.first), m.second);
    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: transpiler <file.proc++> [--no-run] [--show-cpp]\n";
        return 1;
    }

    std::string input_file = argv[1];
    bool no_run = false, show_cpp = false;
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--no-run")   no_run   = true;
        if (arg == "--show-cpp") show_cpp = true;
    }

    std::ifstream in(input_file);
    if (!in) {
        std::cerr << "Error: '" << input_file << "' not found\n";
        return 1;
    }
    std::ostringstream buf;
    buf << in.rdbuf();
    std::string cpp_source = transpile(preprocess(buf.str()));

    std::string base     = input_file.substr(0, input_file.rfind('.'));
    std::string cpp_file = base + ".cpp";
    std::string out_file = base + ".exe";

    std::ofstream out(cpp_file);
    out << cpp_source;
    out.close();

    std::cout << "[proc++] transpiled  " << input_file << " -> " << cpp_file << "\n";

    if (show_cpp) {
        std::cout << "\n" << std::string(40, '-') << " generated C++ "
                  << std::string(40, '-') << "\n" << cpp_source
                  << "\n" << std::string(95, '-') << "\n\n";
    }

    std::string compile_cmd = "g++ \"" + cpp_file + "\" -o \"" + out_file + "\"";
    if (system(compile_cmd.c_str()) != 0) {
        std::cerr << "[proc++] compile failed\n";
        return 1;
    }

    std::cout << "[proc++] compiled    " << cpp_file << " -> " << out_file << "\n";

    if (!no_run) {
        std::cout << "[proc++] running     " << out_file << "\n"
                  << std::string(40, '-') << "\n";
        system(("\"" + out_file + "\"").c_str());
    }

    return 0;
}
