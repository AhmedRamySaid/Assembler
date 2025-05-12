#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <stdexcept>
#include <algorithm>
#include <cctype>

class CustomAssembler {
private:
    // Register mapping
    std::unordered_map<std::string, int> registerMap = {
        {"r0", 0}, {"r1", 1}, {"r2", 2}, {"r3", 3},
        {"r4", 4}, {"r5", 5}, {"r6", 6}, {"r7", 7}
    };

    // Opcode and function code mappings
    std::unordered_map<std::string, std::string> opcodeMap = {
        // R-type instructions (opcode 0000)
        {"add", "0000"},
        {"sub", "0000"},
        {"and", "0000"},
        {"or", "0000"},
        {"slt", "0000"},
        {"mvz", "0000"},

        // I-type instructions
        {"addi", "0100"},
        {"andi", "0101"},
        {"lw", "0110"},
        {"lea", "0111"},
        {"sw", "1000"},
        {"pcm", "1001"},
        {"beq", "1010"},

        // J-type instruction
        {"jump", "1111"}
    };

    std::unordered_map<std::string, std::string> functionCodeMap = {
        {"add", "000"},
        {"sub", "010"},
        {"and", "100"},
        {"or", "101"},
        {"slt", "111"},
        {"mvz", "001"}
    };

    // Convert register name to 3-bit binary
    std::string getRegisterBinary(const std::string& reg) {
        auto it = registerMap.find(reg);
        if (it == registerMap.end()) {
            throw std::runtime_error("Invalid register: " + reg);
        }
        return std::bitset<3>(it->second).to_string();
    }

    // Convert immediate to binary, supporting decimal and binary inputs
    std::string getImmediateBinary(const std::string& imm) {
        int value;
        // Check if binary string
        if (imm.substr(0, 2) == "0b") {
            value = std::stoi(imm.substr(2), nullptr, 2);
        }
        // Check if decimal
        else {
            value = std::stoi(imm);
        }

        // Validate immediate range
        int maxValue = 255;
        int minValue = -256;
        if (value < minValue || value > maxValue) {
            throw std::runtime_error("Immediate out of range: " + imm +
                                     " (max " + std::to_string(maxValue) + ")");
        }

        // Convert to binary string
        return std::bitset<9>(value).to_string();
    }

    // Convert jump to binary, supporting decimal and binary inputs
    std::string getJumpBinary(const std::string& imm) {
        int value;
        // Check if binary string
        if (imm.substr(0, 2) == "0b") {
            value = std::stoi(imm.substr(2), nullptr, 2);
        }
        // Check if decimal
        else {
            value = std::stoi(imm);
        }

        // Validate immediate range
        int maxValue = 4095;
        if (value < 0 || value > maxValue) {
            throw std::runtime_error("Immediate out of range: " + imm +
                                     " (max " + std::to_string(maxValue) + ")");
        }

        // Convert to binary string
        return std::bitset<12>(value).to_string();
    }

    // Parse and assemble a single instruction
    std::string assembleInstruction(const std::string& line) {
        // Remove leading/trailing whitespace
        std::string trimmedLine = line;
        trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t"));
        trimmedLine.erase(trimmedLine.find_last_not_of(" \t") + 1);

        // Skip empty lines and comments
        if (trimmedLine.empty() || trimmedLine[0] == ';') {
            return "";
        }

        // Tokenize the instruction
        std::istringstream iss(trimmedLine);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            // Remove trailing comma
            if (!token.empty() && token.back() == ',') {
                token.pop_back();
            }
            // Remove commented lines
            if (!token.empty() && token.back() == '#') {
                token.pop_back();
                break;
            }
            tokens.push_back(token);
        }

        // Validate instruction
        if (tokens.empty()) {
            return "";
        }

        std::string opcode = tokens[0];
        std::transform(opcode.begin(), opcode.end(), opcode.begin(), ::tolower);

        // Jump instruction (special case)
        if (opcode == "jump") {
            if (tokens.size() != 2) {
                throw std::runtime_error("Invalid jump instruction: " + trimmedLine);
            }
            return "1111000" + getJumpBinary(tokens[1]);
        }

        // R-type instructions
        if (functionCodeMap.count(opcode)) {
            if (tokens.size() != 4) {
                throw std::runtime_error("Invalid R-type instruction: " + trimmedLine);
            }
            return opcodeMap[opcode] +
                   getRegisterBinary(tokens[2]) +
                   getRegisterBinary(tokens[3]) +
                   getRegisterBinary(tokens[1]) +
                   "000" +
                   functionCodeMap[opcode];
        }

        // I-type instructions
        std::vector<std::string> iTypeOpcodes = {
            "addi", "andi", "lw", "lea", "sw", "pcm", "beq"
        };
        if (std::find(iTypeOpcodes.begin(), iTypeOpcodes.end(), opcode) != iTypeOpcodes.end()) {
            if (tokens.size() != 4) {
                throw std::runtime_error("Invalid I-type instruction: " + trimmedLine);
            }
            return opcodeMap[opcode] +
                   getRegisterBinary(tokens[2]) +
                   getRegisterBinary(tokens[1]) +
                   getImmediateBinary(tokens[3]);
        }

        throw std::runtime_error("Unknown instruction: " + trimmedLine);
    }

public:
    // Assemble entire file or string
    std::vector<std::string> assemble(const std::string& input, bool isFilePath = false) {
        std::vector<std::string> machineCode;
        std::istringstream iss;

        machineCode.push_back(std::bitset<19>(0).to_string());

        // Read input from file or string
        if (isFilePath) {
            std::ifstream file(input);
            if (!file.is_open()) {
                throw std::runtime_error("Cannot open file: " + input);
            }

            std::string line;
            while (std::getline(file, line)) {
                try {
                    std::string instruction = assembleInstruction(line);
                    if (!instruction.empty()) {
                        machineCode.push_back(instruction);
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error assembling line '" << line << "': "
                              << e.what() << std::endl;
                }
            }
        } else {
            // Treat input as multiline string
            iss.str(input);
            std::string line;
            while (std::getline(iss, line)) {
                try {
                    std::string instruction = assembleInstruction(line);
                    if (!instruction.empty()) {
                        machineCode.push_back(instruction);
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error assembling line '" << line << "': "
                              << e.what() << std::endl;
                }
            }
        }

        return machineCode;
    }

    // Write machine code to .mem file
    void writeMemFile(const std::vector<std::string>& machineCode,
                      const std::string& filename) {
        std::ofstream memFile(filename);
        if (!memFile.is_open()) {
            throw std::runtime_error("Cannot open output file: " + filename);
        }

        for (const auto& instruction : machineCode) {
            memFile << instruction << std::endl;
        }
        memFile.close();
    }
};

// Example usage and testing
int main() {
    CustomAssembler assembler;

    try {
        // Assemble from file
        std::vector<std::string> machineCode = assembler.assemble("code.asm", true);

        // Print machine code
        std::cout << "Machine Code:\n";
        for (const auto& instruction : machineCode) {
            std::cout << instruction << std::endl;
        }

        // Optional: Write to .mem file
        assembler.writeMemFile(machineCode, "output.mem");
    } catch (const std::exception& e) {
        std::cerr << "Assembler Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}