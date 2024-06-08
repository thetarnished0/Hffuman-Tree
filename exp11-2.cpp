#include "HuffmanTree.cpp"

int main() {
	int leafnum = 0;
	HTNode ht[MAXhufnum];
	HCode hcd[MAXleafnum];
	//ElemType s[MAXleafnum + 1];
	//printf("Initialize:=====================\n");
	//initialize(ht, leafnum);
	
	//读取hfmTree
	readTree(ht, leafnum);

	

	//编码
	InteractiveEncode(ht, hcd, leafnum);
	//译码
	InteractiveDecode(ht, leafnum);
	//格式化打印编码
	printf("\n");
	PrintCode();
	//直观显示hfmTree
	printTree(ht, 2*leafnum - 2, 2*leafnum, leafnum);

	return 0;
}