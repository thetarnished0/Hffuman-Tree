#include <stdio.h>
#include <stdlib.h>
#include <iostream> // ��������/���
#include <fstream>  // �����ļ�����
#include <string>   // ���ڴ����ַ���
#include <sstream>  // �����ַ�����

// ANSIת������������ɫ
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

typedef struct {//Huffman���ṹ
	int index;//���ں���Ľ��ܲ���
	ElemType data;
	double weight;
	int parent;
	int lchild;
	int rchild;
}HTNode;

typedef struct {//�����õĽṹ��
	ElemType cd[N];
		int start;
}HCode;


static void CreateHT(HTNode ht[], int n0) {//����Ҫ���������С
	int i, k, lnode, rnode;
	double min1, min2;
	for (i = 0; i < 2 * n0 - 1; i++) {//��������������ó�ֵΪ-1
		ht[i].index = i;
		ht[i].parent = ht[i].lchild = ht[i].rchild = -1;
	}
	for (i = n0; i <= 2 * n0 - 2; i++) {//����HT��n0-1����֧�ڵ�
		min1 = min2 = 33333;//��ʼֵ������ڱȽ���,��ʱ����weight
		lnode = rnode = -1;//��ʱ���ҽڵ�
		for (k = 0; k <= i - 1; k++) {//ÿ�α���Ҫ������i�ڵ��ǰ�����нڵ�
			if (ht[k].parent == -1) {//�����ǰ�ڵ�û�и��׽ڵ㣬��ô�Ϳ��Խ������뿼��
				if (ht[k].weight < min1) {//����ط����Կ�����������Ϊmin1<min2������ȨС����Ȩ
					min2 = min1; rnode = lnode;
					min1 = ht[k].weight; lnode = k;
				}
				else if (ht[k].weight < min2) {
					min2 = ht[k].weight; rnode = k;
				}
			}
		}
		ht[i].weight = ht[lnode].weight + ht[rnode].weight;
		ht[i].lchild = lnode; ht[i].rchild = rnode;  //ht[i]��parent�ڵ�
		ht[lnode].parent = i; ht[rnode].parent = i;//���õ��Ľڵ�����parent�ڵ�
	}
}

static void initialize(HTNode ht[], int &leafnum) {
	std::cout << YELLOW << "�������ַ�����С: " << RESET;
	scanf_s("%d", &leafnum);
	getchar();  // ������з�

	std::cout << YELLOW << "Notice:  Ϊ�˶��뷽�㣬�����������'*'����ո��ַ�������\n" << RESET;
	for (int i = 0; i < leafnum; ++i) {//�����û�����
		printf("������� %d ���ַ���Ȩֵ:��ͨ���ո�ָ���", i + 1);
		char data;
		double weight;
		scanf_s("%c %lf", &data, 1, &weight);
		getchar();  // ������з�
		ht[i].data = data; ht[i].weight = weight;
	}
	for (int j = leafnum; j < 2 * leafnum - 1; j++) {//��Ч�ַ������
		ht[j].data = '`';
	}

	//������������
	CreateHT(ht, leafnum);

	// ����������д���ļ�
	std::ofstream myfile;
	myfile.open("hfmTree.txt");
	for (int i = 0; i < 2 * leafnum - 1; ++i) {
		myfile << ht[i].index << " " << ht[i].data << " " << ht[i].weight << " " << ht[i].parent << " " << ht[i].lchild << " " << ht[i].rchild << "\n";
	}
	myfile.close();
}

static void CreateHCode(HTNode ht[], HCode hcd[], int n0) {
	int i, f, c;//��ĸ�����ӵ��±�
	HCode hc;//�൱����ʱ�ṹ��
	for (i = 0; i < n0; i++) {//ʵ��Ҫ���б������ǰn0��Ҷ�ӽڵ�
		hc.start = n0; c = i;
		f = ht[i].parent;
		while (f != -1) {
			if (ht[f].lchild == c) {//���������
				hc.cd[hc.start--] = '0';
			}
			else {
				hc.cd[hc.start--] = '1';//������Һ���
			}
			c = f; f = ht[f].parent;//���ӽڵ��parent�ڵ�ı��������Ѱ��
		}
		hc.start++;//����start++����Ϊǰ���start--�ˣ������һλ��ʼ�±������ȷ
		hc.cd[n0 + 1] = '\0';
		hcd[i] = hc;
	}
}

static int encode(HTNode ht[], HCode hcd[], ElemType s[], int n0, int &flag) {
	int i, f, c;//��ĸ�����ӵ��±�
	HCode hc;//�൱����ʱ�ṹ��
	i = 0;
	int index = 0;//��Ӧ�ַ���htmTree�е�index
	int j;
	while (s[i] != '\0') {//����������ַ���
		for (j = 0; j < n0; j++) {//�����鿴�ڴ��е�hfmtree�Ƿ���Զ��û�������ַ�����
			if (s[i] == ' ') {//��������ǿո񣬵�������һ�£��������ļ���ȡ�Ĳ���
				index = 0; break;
			}
			if (ht[j].data == s[i]) {
				index = j; break;
			}
		}
		if (j == n0) {
			std::cout << RED << "Warning:��ǰhfmTree�޷����㴫����ַ����б��룬��鿴��ǰHFMTree�е��ַ���(P),�˶Ժ������룬���߽���hfmTree��ʼ��\n" << RESET;
			flag = 0;
			break;
		}
		//��������µı���
		hc.start = n0; c = index;
		f = ht[index].parent;
		while (f != -1) {
			if (ht[f].lchild == c) {//���������
				hc.cd[hc.start--] = '0';
			}
			else {
				hc.cd[hc.start--] = '1';//������Һ���
			}
			c = f; f = ht[f].parent;//���ӽڵ��parent�ڵ�ı��������Ѱ��
		}
		hc.start++;//����start++����Ϊǰ���start--�ˣ������һλ��ʼ�±������ȷ
		hc.cd[n0 + 1] = '\0';
		hcd[i] = hc;
		i++;//�±��ƶ�
	}
	return i;//countһ���ж��ٸ��ַ���������
}


static void InteractiveEncode(HTNode ht[], HCode hcd[], int leafnum) {
	// ����ʽ����
	int flag = 1;//����ȷ����ǰ�Ƿ����ɹ�
	std::cout << YELLOW << "����������Ҫ������ַ��������ַ����ַ����п��԰����ո�" << RESET;
	ElemType s1[N] = { '\0' };
	int count = 0; // ��¼�ж��ٸ��ַ�������
	fgets(s1, sizeof(s1), stdin); // ʹ��fgets����scanf_s
	// �Ƴ����ܶ���Ļ��з�
	if (s1[strlen(s1) - 1] == '\n') {
		s1[strlen(s1) - 1] = '\0';
	}
	count = encode(ht, hcd, s1, leafnum, flag);
	if (flag == 1) {//����ɹ��Ž���д��ʹ�ӡ
		std::cout << GREEN << "��������" << RESET;
		std::ofstream myfile;
		myfile.open("CodeFile.txt"); // д��
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
	int i, f, j = 0; //j���ڱ����ַ��±���ƶ�
	f = 2 * n0 - 2;//��������Ǹ�����������±�
	i = 0;

	while (code[i] != '\0') {//����code�ַ���
		if (ht[f].lchild != -1 && ht[f].rchild != -1) {//��ǰparent�ڵ�����Һ��Ӿ���Ϊ��
			if (code[i] == '0') {//��
				f = ht[f].lchild;
			}
			else if (code[i] == '1') {//��
				f = ht[f].rchild;
			}
			i++;//����ǰi������,���ƶ�
			if (code[i] == '\0') {//���һ��01�ַ�Ҫ������������ѭ������������
				res[j++] = ht[f].data;//�ҵ�Ҷ�ӽڵ�
				flag = 1;//ֻ����һ���ַ���У�Բ���
			}
		}
		else {
			res[j++] = ht[f].data;//�ҵ�Ҷ�ӽڵ�
			f = 2 * n0 - 2;//parent�ڵ�ص����ϲ�
			flag = 1;
		}
	}
	if (strchr(res, '`') != NULL) {
		flag = 0;
	}
	res[j] = '\0';
}


static void InteractiveDecode(HTNode ht[], int leafnum) {
	//����ʽ����
	int flag = 0;//���ں˶Խ����Ƿ�ɹ�
	std::cout << RED << "Notice:��ȷ��������ı��봮û�л��з�\n" << RESET;
	std::cout << YELLOW << "��������Ҫ����ı��봮��" << RESET;
	ElemType res[N] = { '\0' };//���
	ElemType code[N] = { '\0' };//�û�����
	scanf_s("%s", code, (unsigned)_countof(code));
	getchar();//������з�
	decode(ht, code, res, leafnum, flag);
	if (flag == 1) {
		int n = 0;
		std::ofstream myfile1;
		myfile1.open("TextFile.txt");//д��
		std::cout << GREEN << "��������\n" << RESET;
		while (res[n] != '\0') {
			if (res[n] == '*') {
				printf(" ");//��ȡ�ļ��Ĳ���
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
		std::cout << RED << "������ı��봮�д�����˶Ժ�������!" << RESET;
	}
	printf("\n");
}

static void PrintCode() {
	std::cout << GREEN << "������ķ�ʽ��ʾ������(�˽�������һ�α���ɹ������Ҵ洢���ļ��еĽ��)��\n" << RESET;
	std::ifstream inFile("CodeFile.txt");//���ļ���(in)��ȡ
	std::ofstream outFile("CodePrin.txt");//д��

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

	//���һ����û��50���ַ������⴦��
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
			std::cout << "������";
			outFile << "������";
			indent += "��   ";
		}
		else {
			std::cout << "������";
			outFile << "������";
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
	std::ifstream myfile("hfmTree.txt");//��ȡ���ڴ�
	int i = 0;//ĳ��ht�ڵ�
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
		leafnum = (i + 1) / 2;//ͳ��Ҷ�ӽڵ����
		myfile.close();
	}
	else {
		std::cout << "Unable to open file";
	}
}

static void dispHFMtree(int& leafnum) {
	std::cout << GREEN << "��ǰ��hfmTree:\n" << RESET;
	std::ifstream myfile("hfmTree.txt");//��ȡ
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