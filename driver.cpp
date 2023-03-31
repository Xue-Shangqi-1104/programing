/*
Author: Shangqi Xue
Date: 3/30/2023
Purpose: Reads information from a file and performs the operation stated 
*/
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>
/*
This method uses fstream to take the information from a file and store them in the different variables
Para: string filename 
Return: void
*/
void readFromFile(std::string fileName);
/*
Converts the string value of the input to uint32 and performs the calculation depending on what is inputed
Para: string operation, string operand1, string operand2
Return: void
*/
void performCalc(std::string, std::string, std::string);
/*
Formats and prints out the result of the calculation to the terminal 
Para: string operation, uint32_t operand1, uint32_t operand2, uint32_t result, string overflow
Return: void
*/
void displayResult(std::string, uint32_t, uint32_t, uint32_t, std::string, int, int);
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
    ASR
};
/*
Takes in a string and converts the string into a enum value. Additionally, it states rather or not the operation needs to set the flags
Para: string operation, bool& setFlag
Return: opcode
*/
opcode interpretOpCode(std::string, bool&);

int main(){
    int nFlag, zFlag;

    readFromFile("Programming-Project-1.txt");
    return 0;
}

void readFromFile(std::string fileName){
    std::string operation, operand1, operand2, line;
    std::ifstream inputFile;

    inputFile.open(fileName);
    if(inputFile.is_open()){
        while(inputFile.good()){
            getline(inputFile, line);
            std::istringstream is(line);
            getline(is, operation, ' ');
            if(operation == "NOT" || operation == "NOTS"){
                is >> operand1;
            }else{
                is >> operand1 >> operand2;                
            }
            performCalc(operation, operand1, operand2);
        } 
    }
}

void performCalc(std::string oper, std::string oper1, std::string oper2){
    std::string operation = oper;
    std::string overFlow;
    uint32_t operand1, operand2, result;
    int32_t operand1ASR, resultASR;
    bool needFlag;
    int nFlag = 0, zFlag = 0;

    std::stringstream ss;
    ss << std::hex << oper1;
    ss >> operand1;
    std::stringstream sh;
    sh << std::hex << oper2;
    sh >> operand2;

    opcode op = interpretOpCode(oper, needFlag);

    switch(op){
        case ADD:
            result = operand1 + operand2;
            break;
        case SUB:
            result = operand1 - operand2;
            break;
        case AND:
            result = operand1 & operand2;
            break;
        case NOT:
            result = ~operand1;
            break;
        case ORR:
            result = operand1 | operand2;
            break;  
        case XOR:
            result = operand1 ^ operand2;
            break;
        case LSR:
            result = operand1 >> operand2;
            break;
        case LSL:
            result = operand1 << operand2;
            break;
        case ASR:
            operand1ASR = operand1;
            resultASR = operand1ASR >> operand2;
            break;
    }

    if(needFlag){
        if(op != ASR && (int32_t)result < 0){
            nFlag = 1;
        }else if(op == ASR && resultASR < 0){
            nFlag = 1;
        }

        if(op != ASR && result == 0){
            zFlag = 1;
        }else if(op == ASR && resultASR == 0){
            zFlag = 1;
        }
        needFlag = false;
    }

    // Used for overflow 
    // if(operand1 > 0 && operand2 > UINT32_MAX - operand1){
    //     overFlow = "yes";
    // }else{
    //     overFlow = "no";
    // }

    if(op != ASR){
        displayResult(operation, operand1, operand2, result, overFlow, nFlag, zFlag);
    }else{
        displayResult(operation, operand1ASR, operand2, resultASR, overFlow, nFlag, zFlag);
    }
}

opcode interpretOpCode(std::string op, bool &flag){
    if(op.back() == 'S'){
        flag = true;    
    }
    if(op == "ADD" || op == "ADDS") return ADD;
    if(op == "SUB" || op == "SUBS") return SUB;
    if(op == "AND" || op == "ANDS") return AND;
    if(op == "NOT" || op == "NOTS") return NOT;
    if(op == "ORR" || op == "ORRS") return ORR;
    if(op == "XOR" || op == "XORS") return XOR;
    if(op == "LSR" || op == "LSRS") return LSR;
    if(op == "LSL" || op == "LSLS") return LSL;
    if(op == "ASR" || op == "ASRS") return ASR;
}

void displayResult(std::string oper, uint32_t oper1, uint32_t oper2, uint32_t rs, std::string check, int nFlag, int zFlag){
    if(oper == "NOT" || oper == "NOTS"){
        printf("%-5s %#15X : %#15X \n", oper.c_str(), oper1, rs);
    }else{
        printf("%-5s %#15X %#15X: %#15X \n", oper.c_str(), oper1, oper2, rs);
    }
    printf("N: %d Z: %d \n", nFlag, zFlag);

    //Used for overflow 
    //printf("%s: %11s \n\n", "Overflow", check.c_str());
}