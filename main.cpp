#include <cmath>
#include <cstdio>
#include <string>

const int ROW_SIZE = 16;

class File {
public:
	File(const char* filename) {
		fh = fopen(filename, "rb");
		if (exists()) {
			fseek(fh, 0, SEEK_END);
			m_length = ftell(fh);
			fseek(fh, 0, SEEK_SET);
		}
	}
	bool exists() const {
		return (fh != nullptr);
	};
	size_t length() const {
		return m_length;
	};
	size_t read(uint8_t* buffer, int length) {
		return fread(buffer, 1, length, fh);
	}
	~File() {
		if (fh) {
			fclose(fh);
		}
	}
private:
	FILE* fh;
	size_t m_length;
};

void printRow(size_t offset, uint8_t* buffer, int length, const char* name) {
	printf("0x%08lx: ", offset);
	for (int i = 0; i < length; ++i) {
		printf("%02x ", buffer[i]);
	}
	while (length < ROW_SIZE) {
		printf("   ");
		++length;
	}
	printf("(%s)\n", name);
}

int main(int argc, char** argv) {
	if (argc < 3) {
		printf("%s: missing operand after '%s'\n", argv[0], argv[argc-1]);
		return 1;
	} else if (argc > 3) {
		printf("%s: extra operand '%s'\n", argv[0], argv[3]);
		return 1;
	}

	File file1(argv[1]);
	if (!file1.exists()) {
		printf("%s doesn't exist!\n", argv[1]);
		return 1;
	}

	File file2(argv[2]);
	if (!file2.exists()) {
		printf("%s doesn't exist!\n", argv[2]);
		return 1;
	}

	if (file1.length() > file2.length()) {
		printf("'%s' is longer than '%s' by %ld bytes.\n", argv[1], argv[2], file1.length() - file2.length());
	} else if (file1.length() < file2.length()) {
		printf("'%s' is shorter than '%s' by %ld bytes.\n", argv[1], argv[2], file2.length() - file1.length());
	} else {
		printf("'%s' and '%s' are both %ld bytes long.\n", argv[1], argv[2], file1.length());
	}

	bool diverged = false;
	for (size_t offset = 0; (offset < file1.length() || offset < file2.length()) && !diverged; offset += ROW_SIZE) {
		uint8_t buffer1[ROW_SIZE];
		uint8_t buffer2[ROW_SIZE];
		int len1 = file1.read(buffer1, ROW_SIZE);
		int len2 = file2.read(buffer2, ROW_SIZE);
		int diffOffset = 0;
		int minLength = std::min(len1, len2);

		for (int j = 0; j < minLength; ++j) {
			if (buffer1[j] != buffer2[j]) {
				diverged = true;
				diffOffset = j;
				break;
			}
		}

		if (len1 != len2 && !diverged) {
			diverged = true;
			diffOffset = minLength;
		}

		if (diverged) {
			printf("Files diverge at 0x%08lx:\n", offset + diffOffset);
			printRow(offset, buffer1, len1, argv[1]);
			printRow(offset, buffer2, len2, argv[2]);
			printf("            ");
			for (int j = 0; j < diffOffset; ++j) {
				printf("   ");
			}
			printf("^\n");
		}
	}
	if (!diverged) {
		printf("Files are identical and do not diverge.\n");
	}
}
