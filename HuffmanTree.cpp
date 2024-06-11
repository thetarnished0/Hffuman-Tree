#include <stdio.h>
#include <stdlib.h>
#include <iostream> // 用于输入/输出
#include <fstream>  // 用于文件操作
#include <string>   // 用于处理字符串
#include <sstream>  // 用于字符串流

// ANSI转义序列用于颜色
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define MAXleafnum 128
#define MAXhufnum 2*MAXleafnum
#define N 512

typedef char ElemType;

typedef struct {//Huffman树结构
	int index;//便于后面的解密查找
	ElemType data;
	double weight;
	int parent;
	int lchild;
	int rchild;
}HTNode;

typedef struct {//编码用的结构体
	ElemType cd[N];
		int start;
}HCode;


static void CreateHT(HTNode ht[], int n0) {//不需要传递数组大小
	int i, k, lnode, rnode;
	double min1, min2;
	for (i = 0; i < 2 * n0 - 1; i++) {//所有相关联域设置初值为-1
		ht[i].index = i;
		ht[i].parent = ht[i].lchild = ht[i].rchild = -1;
	}
	for (i = n0; i <= 2 * n0 - 2; i++) {//构造HT的n0-1个分支节点
		min1 = min2 = 33333;//初始值设大用于比较用,临时左右weight
		lnode = rnode = -1;//临时左右节点
		for (k = 0; k <= i - 1; k++) {//每次遍历要创建的i节点的前面所有节点
			if (ht[k].parent == -1) {//如果当前节点没有父亲节点，那么就可以将其纳入考虑
				if (ht[k].weight < min1) {//这个地方可以看出，是设置为min1<min2，即左权小于右权
					min2 = min1; rnode = lnode;
					min1 = ht[k].weight; lnode = k;
				}
				else if (ht[k].weight < min2) {
					min2 = ht[k].weight; rnode = k;
				}
			}
		}
		ht[i].weight = ht[lnode].weight + ht[rnode].weight;
		ht[i].lchild = lnode; ht[i].rchild = rnode;  //ht[i]是parent节点
		ht[lnode].parent = i; ht[rnode].parent = i;//已用掉的节点设置parent节点
	}
}

static void initialize(HTNode ht[], int &leafnum) {
	std::cout << YELLOW << "请输入字符集大小: " << RESET;
	scanf_s("%d", &leafnum);
	getchar();  // 清除换行符

	std::cout << YELLOW << "Notice:  为了读入方便，请用特殊符号'*'替代空格字符的输入\n" << RESET;
	for (int i = 0; i < leafnum; ++i) {//接受用户输入
		printf("请输入第 %d 个字符和权值:（通过空格分隔）", i + 1);
		char data;
		double weight;
		scanf_s("%c %lf", &data, 1, &weight);
		getchar();  // 清除换行符
		ht[i].data = data; ht[i].weight = weight;
	}
	for (int j = leafnum; j < 2 * leafnum - 1; j++) {//无效字符的填充
		ht[j].data = '`';
	}

	//建立哈夫曼树
	CreateHT(ht, leafnum);

	// 将哈夫曼树写入文件
	std::ofstream myfile;
	myfile.open("hfmTree.txt");
	for (int i = 0; i < 2 * leafnum - 1; ++i) {
		myfile << ht[i].index << " " << ht[i].data << " " << ht[i].weight << " " << ht[i].parent << " " << ht[i].lchild << " " << ht[i].rchild << "\n";
	}
	myfile.close();
}

static void CreateHCode(HTNode ht[], HCode hcd[], int n0) {
	int i, f, c;//父母，孩子的下标
	HCode hc;//相当于临时结构了
	for (i = 0; i < n0; i++) {//实际要进行编码的是前n0个叶子节点
		hc.start = n0; c = i;
		f = ht[i].parent;
		while (f != -1) {
			if (ht[f].lchild == c) {//如果是左孩子
				hc.cd[hc.start--] = '0';
			}
			else {
				hc.cd[hc.start--] = '1';//如果是右孩子
			}
			c = f; f = ht[f].parent;//孩子节点和parent节点改变继续向上寻找
		}
		hc.start++;//这里start++是因为前面的start--了，必须进一位起始下标才能正确
		hc.cd[n0 + 1] = '\0';
		hcd[i] = hc;
	}
}

static int encode(HTNode ht[], HCode hcd[], ElemType s[], int n0, int &flag) {
	int i, f, c;//父母，孩子的下标
	HCode hc;//相当于临时结构了
	i = 0;
	int index = 0;//对应字符在htmTree中的index
	int j;
	while (s[i] != '\0') {//遍历传入的字符串
		for (j = 0; j < n0; j++) {//遍历查看内存中的hfmtree是否可以对用户输入的字符编码
			if (s[i] == ' ') {//如果输入是空格，单独处理一下，这里是文件读取的补丁
				index = 0; break;
			}
			if (ht[j].data == s[i]) {
				index = j; break;
			}
		}
		if (j == n0) {
			std::cout << RED << "Warning:当前hfmTree无法对你传入的字符进行编码，请查看当前HFMTree中的字符集(P),核对后再输入，或者进行hfmTree初始化\n" << RESET;
			flag = 0;
			break;
		}
		//正常情况下的编码
		hc.start = n0; c = index;
		f = ht[index].parent;
		while (f != -1) {
			if (ht[f].lchild == c) {//如果是左孩子
				hc.cd[hc.start--] = '0';
			}
			else {
				hc.cd[hc.start--] = '1';//如果是右孩子
			}
			c = f; f = ht[f].parent;//孩子节点和parent节点改变继续向上寻找
		}
		hc.start++;//这里start++是因为前面的start--了，必须进一位起始下标才能正确
		hc.cd[n0 + 1] = '\0';
		hcd[i] = hc;
		i++;//下标移动
	}
	return i;//count一下有多少个字符被编码了
}


static void InteractiveEncode(HTNode ht[], HCode hcd[], int leafnum) {
	// 交互式编码
	int flag = 1;//用于确定当前是否编码成功
	std::cout << YELLOW << "请输入您想要编码的字符串或者字符，字符串中可以包含空格：" << RESET;
	ElemType s1[N] = { '\0' };
	int count = 0; // 记录有多少个字符被编码
	fgets(s1, sizeof(s1), stdin); // 使用fgets代替scanf_s
	// 移除可能读入的换行符
	if (s1[strlen(s1) - 1] == '\n') {
		s1[strlen(s1) - 1] = '\0';
	}
	count = encode(ht, hcd, s1, leafnum, flag);
	if (flag == 1) {//编码成功才进行写入和打印
		std::cout << GREEN << "编码结果：" << RESET;
		std::ofstream myfile;
		myfile.open("CodeFile.txt"); // 写入
		for (int k = 0; k < count; k++) {
			int j = hcd[k].start;
			while (hcd[k].cd[j] != '\0') {
				printf("%c", hcd[k].cd[j]);
				myfile << hcd[k].cd[j];
				j++;
			}
		}
		myfile.close();
	}
	printf("\n");
}

static void decode(HTNode ht[], ElemType code[], ElemType res[], int n0, int &flag) {
	int i, f, j = 0; //j用于编码字符下标的移动
	f = 2 * n0 - 2;//最上面的那个顶点的物理下标
	i = 0;

	while (code[i] != '\0') {//遍历code字符串
		if (ht[f].lchild != -1 && ht[f].rchild != -1) {//当前parent节点的左右孩子均不为空
			if (code[i] == '0') {//左
				f = ht[f].lchild;
			}
			else if (code[i] == '1') {//右
				f = ht[f].rchild;
			}
			i++;//若当前i被用了,才移动
			if (code[i] == '\0') {//最后一个01字符要单独解决，这个循环条件的限制
				res[j++] = ht[f].data;//找到叶子节点
				flag = 1;//只解码一个字符的校对补丁
			}
		}
		else {
			res[j++] = ht[f].data;//找到叶子节点
			f = 2 * n0 - 2;//parent节点回到最上层
			flag = 1;
		}
	}
	if (strchr(res, '`') != NULL) {
		flag = 0;
	}
	res[j] = '\0';
}


static void InteractiveDecode(HTNode ht[], int leafnum) {
	//交互式解码
	int flag = 0;//用于核对解码是否成功
	std::cout << RED << "Notice:请确保您输入的编码串没有换行符\n" << RESET;
	std::cout << YELLOW << "请输入你要解码的编码串：" << RESET;
	ElemType res[N] = { '\0' };//结果
	ElemType code[N] = { '\0' };//用户输入
	scanf_s("%s", code, (unsigned)_countof(code));
	getchar();//清除换行符
	decode(ht, code, res, leafnum, flag);
	if (flag == 1) {
		int n = 0;
		std::ofstream myfile1;
		myfile1.open("TextFile.txt");//写入
		std::cout << GREEN << "解码结果：\n" << RESET;
		while (res[n] != '\0') {
			if (res[n] == '*') {
				printf(" ");//读取文件的补丁
				myfile1 << " ";
				n++;
				continue;
			}
			printf("%c", res[n]);
			myfile1 << res[n];
			n++;
		}
		myfile1.close();
	}
	else {
		std::cout << RED << "你输入的编码串有错误，请核对后再输入!" << RESET;
	}
	printf("\n");
}

static void PrintCode() {
	std::cout << GREEN << "以整洁的方式显示编码结果(此结果是最近一次编码成功，并且存储在文件中的结果)：\n" << RESET;
	std::ifstream inFile("CodeFile.txt");//从文件里(in)读取
	std::ofstream outFile("CodePrin.txt");//写入

	if (!inFile || !outFile) {
		std::cerr << "Unable to open file";
		exit(1);
	}

	char buffer[51] = { 0 }; // 50 characters and the null terminator
	while (inFile.read(buffer, 50)) {
		std::cout << buffer << std::endl;
		outFile << buffer << std::endl;
		memset(buffer, 0, sizeof(buffer)); // clear the buffer
	}

	//最后一行若没有50个字符，额外处理
	std::cout << buffer << std::endl;
	outFile << buffer << std::endl;

	inFile.close();
	outFile.close();
	printf("\n");
}



static void printTree(HTNode ht[], int root, std::ofstream& outFile, std::string indent = "", bool isRight = true) {
	if (root != -1) {
		std::cout << indent;
		outFile << indent;

		if (isRight) {
			std::cout << "├──";
			outFile << "├──";
			indent += "│   ";
		}
		else {
			std::cout << "└──";
			outFile << "└──";
			indent += "    ";
		}

		if (ht[root].data == '`') {
			std::cout << "# (" << ht[root].weight << ")" << std::endl;
			outFile << "# (" << ht[root].weight << ")" << std::endl;
		}
		else {
			std::cout << ht[root].data << " (" << ht[root].weight << ")" << std::endl;
			outFile << ht[root].data << " (" << ht[root].weight << ")" << std::endl;
		}

		printTree(ht, ht[root].rchild, outFile, indent, true);
		printTree(ht, ht[root].lchild, outFile, indent, false);
	}
}

static void readTree(HTNode ht[], int &leafnum) {
	std::ifstream myfile("hfmTree.txt");//读取进内存
	int i = 0;//某个ht节点
	if (myfile.is_open()) {
		std::string line;
		while (getline(myfile, line)) {
			std::istringstream iss(line);
			ElemType data;
			double weight;
			int index, parent, lchild, rchild;
			if (!(iss >> index >> data >> weight >> parent >> lchild >> rchild)) { break; } // error

			// process data
			ht[i].index = index;
			ht[i].data = data;
			ht[i].weight = weight;
			ht[i].parent = parent;
			ht[i].lchild = lchild;
			ht[i].rchild = rchild;
			i++;
		}
		leafnum = (i + 1) / 2;//统计叶子节点个数
		myfile.close();
	}
	else {
		std::cout << "Unable to open file";
	}
}

static void dispHFMtree(int& leafnum) {
	std::cout << GREEN << "当前的hfmTree:\n" << RESET;
	std::ifstream myfile("hfmTree.txt");//读取
	if (myfile.is_open()) {
		std::string line;
		while (getline(myfile, line)) {
			std::istringstream iss(line);
			ElemType data;
			double weight;
			int index, parent, lchild, rchild;
			if (!(iss >> index >> data >> weight >> parent >> lchild >> rchild)) { break; } // error

			// process data
			std::cout << "Index:" << index << ", Data: " << data << ", Weight: " << weight << ", Parent: " << parent << ", LChild: " << lchild << ", RChild: " << rchild << std::endl;
		}
		myfile.close();
	}
	else {
		std::cout << "Unable to open file";
	}
}

static void quit() {
	std::cout << GREEN << "Thank you for using my system!\nBye~~~\n" << RESET;
}