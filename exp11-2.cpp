#include "HuffmanTree.cpp"

int main() {
		char choice;
		int leafnum = 0;
		HTNode ht[MAXhufnum];
		HCode hcd[MAXleafnum];

		std::cout << YELLOW << "*************************************************\n";
		std::cout << "*       Welcome to DIO's Huffman system !       *\n";
		std::cout << "*               Powered by @DIO                 *\n";
		std::cout << "*************************************************\n" << RESET;

		readTree(ht, leafnum);//��ȡhfmTree���ڴ�
		do {
			std::cout << CYAN << "�˵�:\n";
			std::cout << GREEN << "I - ��ʼ����������\n";
			std::cout << RED << "C - ѹ��\n";
			std::cout << BLUE << "D - ��ѹ\n";
			std::cout << MAGENTA << "Q - �˳�\n";
			std::cout << YELLOW << "P - չʾ��ǰ��������\n";
			std::cout << WHITE << "PS:����û�û�н���hfmTree�ĳ�ʼ��������ϵͳ��Ĭ�϶�ȡ�ļ��е�hfmTree���ڴ棬ֱ���û����г�ʼ������\n";
			std::cout << "��ѡ��һ������: " << RESET;
			std::cin >> choice;//char in
			getchar();

			switch (choice) {
			case 'I':
			case 'i':
				initialize(ht, leafnum);
				break;
			case 'C':
			case 'c':
				InteractiveEncode(ht, hcd, leafnum);
				PrintCode();//������ʾ
				break;
			case 'D':
			case 'd':
				InteractiveDecode(ht, leafnum);
				break;
			case 'P':
			case 'p':
				dispHFMtree(leafnum);
				printTree(ht, 2 * leafnum - 2, 2 * leafnum, leafnum);
				break;
			case 'Q':
			case 'q':
				quit();
				break;
			default:
				std::cout << "��Ч��ѡ�����������롣\n";
				break;
			}
		} while (choice != 'Q' && choice != 'q');

		return 0;
}