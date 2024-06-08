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

		readTree(ht, leafnum);//读取hfmTree到内存
		do {
			std::cout << CYAN << "菜单:\n";
			std::cout << GREEN << "I - 初始化哈夫曼树\n";
			std::cout << RED << "C - 压缩\n";
			std::cout << BLUE << "D - 解压\n";
			std::cout << MAGENTA << "Q - 退出\n";
			std::cout << YELLOW << "P - 展示当前哈夫曼树\n";
			std::cout << WHITE << "PS:如果用户没有进行hfmTree的初始化操作，系统将默认读取文件中的hfmTree到内存，直到用户进行初始化操作\n";
			std::cout << "请选择一个操作: " << RESET;
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
				PrintCode();//整洁显示
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
				std::cout << "无效的选择，请重新输入。\n";
				break;
			}
		} while (choice != 'Q' && choice != 'q');

		return 0;
}