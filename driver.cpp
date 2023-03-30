/*
Author: Shangqi Xue
Date: 2/20/2023
Purpose: Reads information from a file and performs the operation stated 
*/


#include <fstream>
#include <sstream>
#include <iostream>

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
void displayResult(std::string, uint32_t, uint32_t, uint32_t, std::string);
enum opcode {
    ADD, ADDS,
    SUB, SUBS,
    AND, ANDS,
    NOT, NOTS,
    ORR, ORRS,
    XOR, XORS,
    LSR, LSRS,
    ASR, ASRS
};
opcode interpretOpCode(std::string);

int main(){
    int nFlag, zFlag;

    readFromFile("Programming-Project-1.txt");
    return 0;
}

void readFromFile(std::string fileName){
    std::string operation, operand1, operand2;
    std::ifstream inputFile;

    inputFile.open(fileName);
    if(inputFile.is_open()){
        while(inputFile.good()){
            inputFile >> operation >> operand1 >> operand2;
            performCalc(operation, operand1, operand2);
        } 
    }
}

void performCalc(std::string oper, std::string oper1, std::string oper2){
    std::string operation = oper;
    std::string overFlow;
    uint32_t operand1, operand2, result;

    std::stringstream ss;
    ss << std::hex << oper1;
    ss >> operand1;
    std::stringstream sh;
    sh << std::hex << oper2;
    sh >> operand2;

    switch(interpretOpCode(oper)){
        case ADD:
            result = operand1 + operand2;
            break;
        case ADDS:
            result = operand1 + operand2;
            
            break;
        case SUB:
            break;
        case SUBS:
            break;
        case AND:
            break;
        case ANDS:
            break;
        case NOT:
            break;
        case NOTS:
            break;
        case ORR:
            break;
        case ORRS:
            break;
        case XOR:
            break;
        case XORS:
            break;
        case LSR:
            break;
        case LSRS:
            break;
        case ASR:
            break;
        case ASRS:
            break;
    }

    // if(operand1 > 0 && operand2 > UINT32_MAX - operand1){
    //     overFlow = "yes";
    // }else{
    //     overFlow = "no";
    // }

    displayResult(operation, operand1, operand2, result, overFlow);
}

opcode interpretOpCode(std::string op){
    if(op == "ADD") return ADD;
    if(op == "ADDS") return ADDS;
    if(op == "SUB") return SUB;
    if(op == "SUBS") return SUBS;
    if(op == "AND") return AND;
    if(op == "ANDS") return ANDS;
    if(op == "NOT") return NOT;
    if(op == "NOTS") return NOTS;
    if(op == "ORR") return ORR;
    if(op == "ORRS") return ORRS;
    if(op == "XOR") return XOR;
    if(op == "XORS") return XORS;
    if(op == "LSR") return LSR;
    if(op == "LSRS") return LSRS;
    if(op == "ASR") return ASR;
    if(op == "ASRS") return ASRS;
}

void displayResult(std::string oper, uint32_t oper1, uint32_t oper2, uint32_t rs, std::string check){
    printf("%-5s %#15X %#15X: %#15X \n", oper.c_str(), oper1, oper2, rs);
    //printf("%s: %11s \n\n", "Overflow", check.c_str());
}