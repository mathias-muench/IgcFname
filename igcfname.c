#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *int2base36(unsigned int n, char *buf)
{
	static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUFWXYZ";

	char *b = buf + 4;
	*--b = 0;

	do {
		if (b == buf)
			return NULL;

		*--b = digits[n % 36];
		n /= 36;
	} while (n);

	return b;
}

static const char b36[] = "0123456789ABCDEFGHIJKLMNOPQRSTUFWXYZ";
static const char *longFormat = "%04d-%02d-%02d-%3s-%3s-%02d.%3s";
static const char *shortFormat = "%1d%c%c%c%3s%c.%s";

static char *l2s(const char *name, char *conv)
{

	char man[4], ser[4], ext[4];
	unsigned yyyy, mm, dd, fl;
	if (7 == sscanf(name, longFormat, &yyyy, &mm, &dd, man, ser, &fl, ext)) {
		char *c;
		for (c = ser; *c; c++) {
			*c = toupper(*c);
		}
		sprintf(conv, shortFormat, yyyy - 2010, b36[mm], b36[dd], man[0], ser, b36[fl], ext);
	} else {
		*conv = '\0';
	}
	return conv;
}

unsigned b10(char c)
{
	return strchr(b36, c) - b36;
}

const char *man(char c)
{
	return "MAN";
}

static char *s2l(const char *name, char *conv)
{
	char ser[4], ext[4];
	unsigned y;
	char m, d, c, f;

	if (7 == sscanf(name, shortFormat, &y, &m, &d, &c, ser, &f, ext)) {
		sprintf(conv, longFormat, y + 2010, b10(m), b10(d), man(c), ser, b10(f), ext);
	} else {
		*conv = '\0';
	}
	return conv;
}

int main(int argc, char *argv[])
{
	argv++;
	while (*argv) {
		const char *name = *argv++;
		char conv[FILENAME_MAX];

		if (strlen(name) == 12) {
			printf("%s\n", s2l(name, conv));
		} else if (strlen(name) == 25) {
			printf("%s\n", l2s(name, conv));
		}
	}
}

/* vi:ai:sw=4:ts=4:et
*/
