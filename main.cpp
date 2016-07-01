
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <exception>      // std::exception

using namespace std;


//All the Declaration of my functions
vector<string> solve(vector<int> question, vector<char> operands);
void looper(int total, int target, string answerList, vector<int> set, vector<char> operands);
string opAlgoFormat(vector<int> set, vector<char> values2);
int chrToValue(int n1, char op, int n2);
vector<int> getQuestion();
void outputAnswer(vector<string> answer);
string checkAnswer(int targetAnswer, vector<char> availableOperands, vector<int> numberSequence, vector<int> operandSequence);
bool checkOperandPriority(char previous, char newChar);
string convertVectorToString(vector<int> numbers, vector<int> operations, vector<char> availableChar);
vector<int> adjustPositionForNextQuestion(vector<int> adjustment);
vector<int> formulateNewQuestionPosition(vector<int> originalQuestion, vector<int> adjustment, int currentPosition);


//Declare Globa Constant
const string problemFile = "Problem.txt";
const string answerFile = "answer.txt";


//Main Program
int main() {

	vector<int> question = getQuestion();	//Retrieve Question from file
	vector<char> availableOperands = { '*','/','+','-' };	//initialize how many available operands in this program (Order of the operands are very important)
	vector<string> answer;
	int inputValue;
	if (question.size() == 0) {
		answer.push_back("-1");
	}
	else {

		answer = solve(question, availableOperands);
	}
	
	outputAnswer(answer);	//Output the answer to answer.txt
	return 0;
}

/***
	getQuestion()
	- Open Problem file and retrieve all the numbers from the file
	return:
	- vector<int> - List of Numbers

*/
vector<int> getQuestion() {
	vector<int> numbers;
	string line;
	ifstream opener;
	opener.open(problemFile);
	if (opener.is_open()) {

		while (getline(opener, line))
		{
			numbers.push_back(stoi(line));
		}
	}

	opener.close();
	return numbers;
	
}

/***
	outputAnswer(vector<string>)
	- Open Answer file and write all the answers into the file
	return:
	- Void

*/
void outputAnswer(vector<string> answer) {
	ofstream opener(answerFile);
	if (answer.empty()) {
		opener << "-1" << endl;
	}
	else {
		for (vector<string>::iterator it = answer.begin(); it != answer.end();it++) {
			opener << *it << endl;
		}
	}
	opener.close();
}

/***
	vector<string> solve(vector<int> question, vector<char> availableOperands)
	- get the question and available operands
	- solving operation mainly perform in this function
	return:
	- vector<string> - answer list

*/
vector<string> solve(vector<int> question, vector<char> operands) {

	int targetAnswer = question.back();	//Get the final answer
	question.pop_back();	//remove the final answer from question list
	long int totalLoopingCount_Operands = pow(operands.size(),(question.size() - 1));	// Calculate total looping numbers(operands)
	long int totalLoopingCount_Questions = 1;
	bool isRedundantAnswer;
	vector<string> answer;
	vector<int> tempQuestion;
	vector<int> tempQuestionPositionAdjust(question.size(),0);
	
	//Calculate the total Looping count need to be perform for constructing new equation(numbers)
	for (int i = 0; i < question.size(); i++) {
		totalLoopingCount_Questions *= (question.size() - i);
	}
	for (long int k = 0; k < totalLoopingCount_Questions; k++) {
		isRedundantAnswer = false;
		vector<int> operationSequence;
		int currentIndex = k % question.size();
		tempQuestion = formulateNewQuestionPosition(question, tempQuestionPositionAdjust,currentIndex);
		if (k != totalLoopingCount_Questions - 1) {
			tempQuestionPositionAdjust = adjustPositionForNextQuestion(tempQuestionPositionAdjust);

		}
		//Fill up the operation sequence with first priority operands (int this case '*')
		for (int i = 0; i < question.size() - 1; i++) {
			operationSequence.push_back(0);
		}
		totalLoopingCount_Operands--;	//minus the total looping count because we already generate one of the possible sequence

										//Start the answer seeking algorithm here
		for (long int i = 0; i < totalLoopingCount_Operands; i++) {
			string checkResult = checkAnswer(targetAnswer, operands, tempQuestion, operationSequence);	//Check the current equation
			//check redundant answer
			for (vector<string>::iterator it = answer.begin(); it != answer.end();it++) {
				if (*it == checkResult) {
					isRedundantAnswer = true;
				}
			}
			//if result == -1, means this is not a solution
			if (checkResult != "-1" && !isRedundantAnswer) {
				answer.push_back(checkResult);	//insert the answer into the list
			}
			//increment the operationSequence will change the equation
			operationSequence[0]++;

			/*
			- this for loop is a bit difficult to understand
			- so just run this code in a new project and u might be able to understand what am i doing
			Example Code:

			#include <iostream>

			using namespace std;

			int main() {


			int numbers[] = { 0,0,0,0,0 };
			for (int j = 0; j < 1000; j++) {
			numbers[0]++;
			for (int i = 0; i < 5;i++) {
			if (numbers[i] >= 5) {
			numbers[i] = 0;
			numbers[i+1] ++;
			}
			}
			for (int i = 0; i < 5; i++) {
			cout << numbers[i];
			}
			cout << endl;
			}

			system("PAUSE");
			return 0;
			}

			*/
			for (int j = 0; j < question.size() - 1; j++) {
				if (operationSequence[j] == operands.size()) {

					operationSequence[j] = 0;
					operationSequence[j + 1] ++;
				}
			}
			if (operationSequence[i % (question.size() - 1)] == 5) {
				cout << "error" << endl;
				break;
			}
		}


		
	}
	return answer;
}

/*
	formulateNewQuestionPosition(vector<int> originalQuestion, vector<int> adjustment)
	- Formulate a new question
	Return:
	-	vector<int> - question with different position
*/
vector<int> formulateNewQuestionPosition(vector<int> originalQuestion, vector<int> adjustment,int currentPosition) {
	vector<int> newQuestion = originalQuestion;
	vector<int> tempAdjustment = adjustment;
	for (int j = adjustment.size()-1; j >=0 ;j--) {
		for (int i = 0; i < tempAdjustment[j];i++) {
			int tempVal = newQuestion[i+j];
			newQuestion[i+j] = newQuestion[i+j + 1];
			newQuestion[i +j+ 1] = tempVal;
		}
	}
	return newQuestion;
}
/*
adjustPositionForNextQuestion(vector<int> adjustment)
- calculate new position adjustment for next question usage
Return:
-	vector<int> - adjustment of position of numbers for next question
*/
vector<int> adjustPositionForNextQuestion(vector<int> adjustment) {
	vector<int> newAdjustment = adjustment;
	newAdjustment[0]++;
		for (int i = 0; i < adjustment.size(); i++) {

			if ((newAdjustment[i] + i) == newAdjustment.size()) {
				newAdjustment[i] = 0;
				newAdjustment[i + 1]++;
			}
		}
	
	return newAdjustment;
}
/*
checkAnswer(int targetAnswer,vector<char> availableOperands, vector<int> numberSequence, vector<int> operandSequence)
-	it will check whether the equation correct or not
Return:
-	string - Answer String in a formalized way
*/
string checkAnswer(int targetAnswer,vector<char> availableOperands, vector<int> numberSequence, vector<int> operandSequence) {
	vector<int> operationSequence;		//Create a list to indicate calculate which operand first
	vector<int> tempNumberSequence = numberSequence;	//Create a copy of question list
	vector<int> tempOperandSequence = operandSequence;	//Create a copy of operands list

	//Find out which part of the equation should be calculate first
	for (int i = 0; i < availableOperands.size(); i++) {
		for (int j = 0; j < operandSequence.size();j++) {
			if (operandSequence.at(j) == i) {
				operationSequence.push_back(j);
			}
		}
	}
	//Start the equation checking algorithm
	for (int i = 0; i < operandSequence.size();i++) {
		int currentSequenceNumber = operationSequence[i];	//get the location of operands
		int tempValue = chrToValue(tempNumberSequence[currentSequenceNumber], availableOperands[tempOperandSequence[currentSequenceNumber]], tempNumberSequence[currentSequenceNumber + 1]);	//Calculate the value
		//if value == -1, means the equation is not valid
		if (tempValue == -1) {
			return "-1";
		}
		tempNumberSequence.erase(tempNumberSequence.begin()+ currentSequenceNumber, tempNumberSequence.begin()+ currentSequenceNumber +2);	//remove those 2 calculated numbers in question list
		tempNumberSequence.insert(tempNumberSequence.begin()+ currentSequenceNumber, tempValue);	//Insert the result into the list again
		tempNumberSequence.shrink_to_fit();	//Shrink the size of the list
		tempOperandSequence.erase(tempOperandSequence.begin() + currentSequenceNumber);	//remove the operands in the list
		tempOperandSequence.shrink_to_fit();	//Shrink the size of the list
		//This will be looking for those position behind currentSequenceNumber and decrement those numbers to fit the logic
		/*
			Example:
			1. Create list - 1 2 3 4 5 6 7
			2. Remove '4'
			3. list - 1 2 3 5 6 7
			4. Sort list - 1 2 3 4 5 6

			- Step 1 - 3 were done at above
			- Step 4 is what this for loop doing
			*/
		for (int j = 0; j < operationSequence.size();j++) {
			if (operationSequence[j] > currentSequenceNumber) {
				operationSequence[j]--;
			}
		}
	}
	//If found the answer then return equation in a formalized way
	//otherwise just return -1
	if (tempNumberSequence[0] == targetAnswer) {
		string tempValue = convertVectorToString(numberSequence, operandSequence, availableOperands);
		//start to check is there any redundant answers
		return tempValue;
	}
	else {
		return "-1";
	}
}
/*
	convertVectorToString(vector<int> numbers , vector<int> operations,vector<char> availableChar)
	- Combine Operands and Numbers to make the output look nicer
	Return:
	-	string - return the nice looking output
*/
string convertVectorToString(vector<int> numbers , vector<int> operations,vector<char> availableChar) {
	string tempString = "";
	for (int i = 0; i < numbers.size(); i++) {
		tempString += to_string(numbers[i]);
		if (i < operations.size()) {
			tempString += availableChar[operations[i]];
		}
	}
	return tempString;
}

/*
	chrToValue(int n1, char op, int n2)
	- Based on the operands, perform different calculation
	Return:
	-	int - calculated result
*/
int chrToValue(int n1, char op, int n2) {
	//cout << "v1=" << n1 << " op= " << op << " v2=" << n2 << endl;
	switch (op) {
	case '+':
		return n1 + n2;
	case '-':
		return n1 - n2;
	case '/':
		if (n1 % n2 == 0 && n2 != 0) {
			return n1 / n2;
		}
		return -1;
	case '*':
		return n1 * n2;
	}
	return -1;
}