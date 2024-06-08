#include "HuffmanTree.cpp"

int main() {
	int leafnum = 0;
	HTNode ht[MAXhufnum];
	HCode hcd[MAXleafnum];
	//ElemType s[MAXleafnum + 1];
	//printf("Initialize:=====================\n");
	//initialize(ht, leafnum);
	
	//��ȡhfmTree
	readTree(ht, leafnum);

	

	//����
	InteractiveEncode(ht, hcd, leafnum);
	//����
	InteractiveDecode(ht, leafnum);
	//��ʽ����ӡ����
	printf("\n");
	PrintCode();
	//ֱ����ʾhfmTree
	printTree(ht, 2*leafnum - 2, 2*leafnum, leafnum);

	return 0;
}