/*
Author: Shangqi Xue
Date: 3/30/2023
Purpose: Reads information from a file and performs the operation stated 
*/
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>
#include <unordered_map>
#include<bits/stdc++.h>

/*
This method uses fstream to take the information from a file and store them in the different variables
Para: string filename 
Return: void
*/
void readFromFile(std::string fileName, std::unordered_map<std::string, uint32_t>&, int[]);
/*
Converts the string value of the input to uint32 and performs the calculation depending on what is inputed
Para: string operation, string operand1, string operand2
Return: void
*/
void performCalc(std::string, std::string, std::string, std::string, std::unordered_map<std::string, uint32_t>&, int[]);
/*
Formats and prints out the result of the calculation to the terminal 
Para: string operation, uint32_t operand1, uint32_t operand2, uint32_t result, string overflow
Return: void
*/
void displayResult(std::string, std::string, std::string, std::string, int[], std::unordered_map<std::string, uint32_t>&);
/*
Uses enums to list the different possible opcode that can exist inside the file.
*/
enum opcode {
    ADD, 
    SUB, 
    AND, 
    NOT, 
    ORR, 
    XOR, 
    LSR, 
    LSL,
    ASR,
    MOV
};
/*
Takes in a string and converts the string into a enum value. Additionally, it states rather or not the operation needs to set the flags
Para: string operation, bool& setFlag
Return: opcode
*/
opcode interpretOpCode(std::string, bool&);

int main(){
    int flags[4] = {0,0,0,0};
    std::unordered_map<std::string, uint32_t> registers;
    registers["R0"] = 0;
    registers["R1"] = 0;
    registers["R2"] = 0;
    registers["R3"] = 0;
    registers["R4"] = 0;
    registers["R5"] = 0;
    registers["R6"] = 0;
    registers["R7"] = 0;

    readFromFile("Programming-Project-1.txt", registers, flags);
    return 0;
}

void readFromFile(std::string fileName, std::unordered_map<std::string, uint32_t> &registers, int flags[]){
    std::string operation, saveTo, operand1, operand2, line;
    std::ifstream inputFile;

    inputFile.open(fileName);
    if(inputFile.is_open()){
        while(inputFile.good()){
            getline(inputFile, line);
            std::istringstream is(line);
            getline(is, operation, ' ');
            if(operation == "NOT" || operation == "NOTS"){
                is >> operand1;
            }else if (operation == "MOV" || operation == "mov"){
                is >> saveTo >> operand1;
            }
            else{
                is >> saveTo >> operand1 >> operand2;                
                operand1 = operand1.substr(0,2);
                transform(operand1.begin(), operand1.end(), operand1.begin(), ::toupper);
                transform(operand2.begin(), operand2.end(), operand2.begin(), ::toupper);
            }

            saveTo = saveTo.substr(0,2);
            transform(saveTo.begin(), saveTo.end(), saveTo.begin(), ::toupper);

            performCalc(operation, saveTo, operand1, operand2, registers, flags);
        } 
    }
}

void performCalc(std::string oper, std::string save, std::string oper1, std::string oper2, std::unordered_map<std::string, uint32_t> &registers, int flags[]){
    std::string operation = oper;
    uint32_t immVal = 0, cFlagHolder; 
    //int32_t operand1ASR, resultASR;
    bool needFlag = false;

    opcode op = interpretOpCode(oper, needFlag);

    if(op == MOV){
        std::stringstream ss;
        ss << std::hex << oper1.substr(1,10);
        ss >> immVal;
    }

    if(op == LSR || op == LSL){
        std::stringstream ss;
        ss << std::hex << oper2.substr(1,2);
        ss >> immVal;
    }

    switch(op){
        case MOV:
            registers[save] = immVal;
            break;
        case ADD:
            registers[save] = registers[oper1] + registers[oper2];
            break;
        case SUB:
            registers[save] = registers[oper1] - registers[oper2];
            break;
        case AND:
            registers[save] = registers[oper1] & registers[oper2];
            break;
        case NOT:
            registers[save] = ~registers[oper1];
            break;
        case ORR:
            registers[save] = registers[oper1] | registers[oper2];
            break;  
        case XOR:
            registers[save] = registers[oper1] ^ registers[oper2];
            break;
        case LSR:
            cFlagHolder = registers[oper1];
            registers[save] = registers[oper1] >> immVal;
            break;
        case LSL:
            cFlagHolder = registers[oper1];
            registers[save] = registers[oper1] << immVal;
            break;

        //not tested for this check point 
        // case ASR:
        //     operand1ASR = operand1;
        //     resultASR = operand1ASR >> operand2;
        //     break;
    }

    if(needFlag){
        //test for the n flag

        if((int32_t)registers[save] < 0){
            flags[0] = 1;
        }else{
            flags[0] = 0;
        }

        //test of the z flag
        if(registers[save] == 0){
            flags[1] = 1;
        }else{
            flags[1] = 0;
        }

        //test for the c flag
        if((op == ADD && (uint64_t)registers[oper1] + (uint64_t)registers[oper2] > UINT32_MAX) || (op == SUB && registers[oper1] >= registers[oper2])){
            flags[2] = 1;
        }else if(op == LSR || op == LSL){
            uint32_t shifted, lastBit;
            if(op == LSR){
                shifted = cFlagHolder >> immVal;
                lastBit = shifted & 1;
            }else{
                shifted = cFlagHolder << immVal;
                lastBit = shifted & 1;
            }
            flags[2] = lastBit;
        }else{
            flags[2] = flags[2];
        }

        //test for the v flag
        if(op != LSL && op != LSR && registers[oper1] > 0 && (registers[oper2] > INT32_MAX - registers[oper1] || registers[oper2] < INT32_MAX - registers[oper1] )){
            flags[3] = 1;
        }else{
            flags[3] = flags[3];
        }
        needFlag = false;
    }

    if(op != ASR){
        displayResult(operation, save, oper1, oper2, flags, registers);
    }
    // else{
    //     displayResult(operation, operand1ASR, operand2, resultASR, overFlow, flags);
    // }
}

opcode interpretOpCode(std::string op, bool &flag){
    if(op.back() == 'S' || op.back() == 's'){
        flag = true;    
    }

    transform(op.begin(), op.end(), op.begin(), ::toupper);

    if(op == "ADD" || op == "ADDS") return ADD;
    if(op == "SUB" || op == "SUBS") return SUB;
    if(op == "AND" || op == "ANDS") return AND;
    if(op == "NOT" || op == "NOTS") return NOT;
    if(op == "ORR" || op == "ORRS") return ORR;
    if(op == "XOR" || op == "XORS") return XOR;
    if(op == "LSR" || op == "LSRS") return LSR;
    if(op == "LSL" || op == "LSLS") return LSL;
    if(op == "ASR" || op == "ASRS") return ASR;
    if(op == "MOV") return MOV;
}

void displayResult(std::string oper, std::string save, std::string oper1, std::string oper2, int flags[], std::unordered_map<std::string, uint32_t> &registers){
    if(oper == "MOV" || oper == "mov"){
        printf("%-5s %15s , %15s \n", oper.c_str(), save.c_str(), oper1.c_str());
    }else{
        printf("%-5s %15s , %15s , %15s \n", oper.c_str(), save.c_str(), oper1.c_str(), oper2.c_str());
    }
    printf("R0: %#15X R1: %#15X R2: %#15X R3: %#15X \nR4: %#15X R5: %#15X R6: %#15X R7: %#15X \n", 
            registers["R0"], registers["R1"], registers["R2"], registers["R3"], registers["R4"], 
            registers["R5"], registers["R6"], registers["R7"]);
    printf("N: %d Z: %d C: %d V: %d \n\n", flags[0], flags[1], flags[2], flags[3]);
}