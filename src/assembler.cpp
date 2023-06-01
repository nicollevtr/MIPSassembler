#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <bitset>
using namespace std;

long long decimalToBinary(int n) {
  long long bin = 0;
  int rem, i = 1;

  while (n!=0) {
    rem = n % 2;
    n /= 2;
    bin += rem * i;
    i *= 10;
  }

  return bin;
}

class instructionRead{
public:
	bitset<5> rs;
	bitset<5> rt;
	bitset<5> rd;
	int addr;
	int format;
	int offset;
	string text;
	string macro;
	string label;
	string loop;
	bitset<6> inst_31_26;
	bitset<5> inst_25_21;
	bitset<5> inst_20_16;
	bitset<16> inst_15_0;
	bitset<5> inst_15_11;
	bitset<11> inst_10_0;
	string opcode;
	unsigned long hexcode;
	void clear();
};
void instructionRead::clear(){
	rs = 0;
	rd = 0;
	rt = 0;
	addr = 0;
	format = 0;
	offset = 0;
	text = "";
	macro = "";
	label = "";
	loop = "";
	inst_31_26 = 0;
	inst_25_21 = 0;
	inst_20_16 = 0;
	inst_15_0 = 0;
	inst_15_11 = 0;
	inst_10_0 = 0;
	opcode = "";
}

int main() {
	vector<instructionRead> instructions;
	instructionRead instruction;
	string line;
	ifstream readFrom;
	readFrom.open("program.asm");
	int addr = 0;
	int count = 0;
	bool jumpMacro = false;
	do{
		if(!jumpMacro)
			getline(readFrom, line);
		if(readFrom.eof()) break;
		int foundComment = line.find("#");
		if(foundComment)
			line = line.substr(0, foundComment);
		instruction.text = line;
		//cout << "instruction: " <<  instructions.at(addr).text << endl;
		int foundType = line.find(" ");
		string type = line.substr(0, foundType);
		int format;
		jumpMacro = false;
		if(type == "add" || type == "sub" ||type == "or" ||type == "and"){
			instruction.format = 0;
			line = line.substr(foundType+3,line.length());
			int foundRd = line.find(", $r");
			instruction.rd = stoi(line.substr(0, foundRd));
			//cout << "rd: " << instruction.rd << endl;
			line = line.substr(foundRd+4,line.length());
			int foundRs = line.find(", $r");
			instruction.rs = stoi(line.substr(0, foundRs));
			//cout << "rs: " <<  instruction.rs << endl;
			line = line.substr(foundRs+4,line.length());
			int foundRt = line.find(", $r");
			instruction.rt= stoi(line.substr(0, foundRt));
			//cout << "rt: " <<  instruction.rt << endl;
			instruction.inst_31_26 = decimalToBinary(0);
			instruction.inst_25_21 = instruction.rs;
			instruction.inst_20_16 = instruction.rt;
			instruction.inst_15_11 = instruction.rd;
			instruction.inst_10_0 = 0;
			int control = 0;
			if(type == "add") //control = 32;
				instruction.inst_10_0 = 0b00000100000;
				//instruction.inst_10_0 = decimalToBinary(32);
			else if(type == "sub") instruction.inst_10_0 = 0b00000100010;
				//instruction.inst_10_0 = decimalToBinary(34);
			else if(type == "and") instruction.inst_10_0 = 0b00000100100;
				//instruction.inst_10_0 = decimalToBinary(36);
			else if(type == "or") instruction.inst_10_0 = 0b00000100001;
			else{

			}
				//instruction.inst_10_0 = (bitset<11>)decimalToBinary(control);
//			instruction.opcode = to_string(instruction.inst_31_26) +
//					to_string(instruction.inst_25_21) +
//					to_string(instruction.inst_20_16) +
//					to_string(instruction.inst_15_11) +
//					to_string(instruction.inst_10_0);
				instruction.opcode = instruction.inst_31_26.to_string() +
						instruction.inst_25_21.to_string() +
						instruction.inst_20_16.to_string() +
						instruction.inst_15_11.to_string() +
						instruction.inst_10_0.to_string();
//				printf("10_0 %s\n",instruction.inst_10_0.to_string().c_str());
//			cout << instruction.inst_31_26<< endl;
//			cout << instruction.inst_25_21<< endl;
//			cout << instruction.inst_20_16<< endl;
//			cout << instruction.inst_15_11<< endl;
//			cout << instruction.inst_10_0<< endl;
			instruction.addr = addr;
			addr++;
		}else if(type == "lw" || type == "sw"){
			instruction.format = 1;
			line = line.substr(foundType+3,line.length());
			int foundRt = line.find(", ");
			instruction.rt= stoi(line.substr(0, foundRt));
			//cout << "rt: " << instruction.rt << endl;
			line = line.substr(foundRt+2,line.length());
			int foundOffset = line.find("($r");
			instruction.offset= stoi(line.substr(0, foundOffset));
			//cout << "offset: " <<instruction.offset << endl;
			line = line.substr(foundOffset+3,line.length());
			int foundRs = line.find(")");
			instruction.rs = stoi(line.substr(0, foundRs));
			//cout << "rs: " <<  instruction.rs << endl;
			if(type == "lw")
				instruction.inst_31_26 = 0b110001;
			else if(type == "sw")
				instruction.inst_31_26 = 0b110101;
			instruction.inst_25_21 = instruction.rs;
			instruction.inst_20_16 = instruction.rt;
			instruction.inst_15_0 = instruction.offset;

			instruction.opcode = instruction.inst_31_26.to_string() +
					instruction.inst_25_21.to_string() +
					instruction.inst_20_16.to_string() +
					instruction.inst_15_0.to_string();
			instruction.addr = addr;
			addr++;
		}else if(type == "jump"){
			instruction.format = 2;
			line = line.substr(foundType+1,line.length());
			int foundLoop = line.find(";");
			instruction.loop = line.substr(0, foundLoop);
			//cout << "loop: " <<  instruction.loop << endl;
			instruction.inst_31_26 = 0b010000;
			instruction.inst_25_21 = decimalToBinary(0);
			instruction.inst_20_16 = decimalToBinary(0);
			//instruction.inst_15_0 = decimalToBinary(0);
			instruction.opcode = instruction.inst_31_26.to_string() +
					instruction.inst_25_21.to_string() +
					instruction.inst_20_16.to_string() ;
			instruction.addr = addr;
			addr++;
		}else if(type == "beq"){
			instruction.format = 3;
			line = line.substr(foundType+3,line.length());
			int foundRs = line.find(", $r");
			instruction.rs = stoi(line.substr(0, foundRs));
			//cout << "rs: " <<  instruction.rs << endl;
			line = line.substr(foundRs+4,line.length());
			int foundRt = line.find(", ");
			instruction.rt= stoi(line.substr(0, foundRt));
			//cout << "rt: " <<  instruction.rt << endl;
			line = line.substr(foundRt+2,line.length());
			int foundLabel = line.find(";");
			instruction.label = line.substr(0, foundLabel);
			//cout << "label: " <<  instruction.label << "." << endl;
			instruction.inst_31_26 = 0b1000;
			instruction.inst_25_21 = instruction.rs;
			instruction.inst_20_16 = instruction.rt;
			instruction.inst_15_0 = decimalToBinary(0);
			instruction.opcode = instruction.inst_31_26.to_string() +
					instruction.inst_25_21.to_string() +
					instruction.inst_20_16.to_string() ;
			instruction.addr = addr;
			addr++;
		}else{
			instruction.format = -1;
			int foundMacro = line.find(":");
			if(foundMacro){
				instruction.macro = line.substr(0,foundMacro);
				//cout << "macro: " <<  instruction.macro << endl;
				jumpMacro = true;
				getline(readFrom, line);
				vector<string> jumpTo;
				jumpTo.push_back(line);
				//loop_addr = addr+1;
				//cout << "jump to: " <<  line << endl;
			}
		}
		instructions.push_back(instruction);
		instruction.clear();
		count ++;
	}while(!readFrom.eof());
	int loop_addr = 0;
	for(int index = 0;index<count; index++){
//		cout << "code: " <<  instructions.at(index).text << endl;
//		if(instructions.at(index).format == -1){
//			string macro_text = instructions.at(index).macro;
//			loop_addr = index + 1;
//		}
		if(instructions.at(index).format == 3){//beq
			for(int search = 0;search < count; search++){
				if((instructions.at(search).macro == instructions.at(index).label)&&(instructions.at(search).format == -1)) {
					instructions.at(index).inst_15_0 = instructions.at(search+1).addr;
					instructions.at(index).opcode = instructions.at(index).opcode + instructions.at(index).inst_15_0.to_string();
				}else{
					//cout << "else beq" << endl;
				}
			}
		}else if(instructions.at(index).format == 2){//jump
			for(int search = index;search >=0; search--){
				if((instructions.at(search).macro == instructions.at(index).loop)&&(instructions.at(search).format == -1)) {
					//instructions.at(index).inst_15_0 = decimalToBinary(instructions.at(search+1).addr);
					instructions.at(index).inst_15_0 = instructions.at(search+1).addr;
					instructions.at(index).opcode = instructions.at(index).opcode + instructions.at(index).inst_15_0.to_string();
				}else{
					//cout << "else jump" << endl;
				}
			}
		}
	}
    FILE *fp;
    fp = fopen("inst_mem.hex","w");
    fprintf(fp, "v2.0 raw\n");
	for(int index = 0;index<count; index++){
		cout << "code: " <<  instructions.at(index).text << endl;
		cout << "opcode: " <<  instructions.at(index).opcode << endl;
		if(instructions.at(index).format>=0){
			instruction.hexcode= stoul(instructions.at(index).opcode, NULL, 2);
			printf("Representação hexadecimal: 0x%lX\n", instruction.hexcode);
			printf("Endereço: 0x%lX\n", instructions.at(index).addr);
			fprintf(fp, "%lX\n", instruction.hexcode);

		}
	}
	return 0;
}
