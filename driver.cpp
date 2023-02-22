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

int main(){
    
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

    if(operation == "ADD"){
        result = operand1 + operand2;
    }

    if(operand1 > 0 && operand2 > UINT32_MAX - operand1){
        overFlow = "yes";
    }else{
        overFlow = "no";
    }

    displayResult(operation, operand1, operand2, result, overFlow);
}

void displayResult(std::string oper, uint32_t oper1, uint32_t oper2, uint32_t rs, std::string check){
    printf("%-5s %#15X %#15X: %#15X \n", oper.c_str(), oper1, oper2, rs);
    printf("%s: %11s \n\n", "Overflow", check.c_str());
}