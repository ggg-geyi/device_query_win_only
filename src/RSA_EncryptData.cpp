#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <openssl/rsa.h>

using namespace std;

#pragma comment(lib, "libeay32.lib")  
#pragma comment(lib, "ssleay32.lib") 

int my_rsa_entrypt(unsigned char *in, unsigned char* out)
{
	RSA *r;
	BIGNUM *bne, *bnn;
	const char *MODULUS = "e1885f94882d3af43945de523bca47630d2d16dc98ee27a4d174c63cf42e3d616cddbeee864c1fddf3ff1b00d2b5dbcf9463c5c71259cd2d53cab02a82a50047";
	unsigned  char *encData;
	int ret;
	unsigned int e = 65537;
	//构建RSA数据结构
	bne = BN_new();
	bnn = BN_new();
	BN_set_word(bne, e);
	BN_hex2bn(&bnn, MODULUS);
	
	r = RSA_new();
	r->e = bne;
	r->n = bnn;

	int flen = RSA_size(r);
	encData = (unsigned char *)malloc(flen);
	memset(encData, 0, flen);
	ret = RSA_public_encrypt(flen, in, encData, r, RSA_NO_PADDING);
	if (ret < 0) {
		printf("Encrypt failed!/n");
		return -1;
	}
	//下面是将密文转为16进制字符串
	BIGNUM *rs;
	rs = BN_new();
	BN_bin2bn((unsigned char *)encData, ret, rs);
	char *tmpData2 = BN_bn2hex(rs); //将密文转为16进制字符串
  memcpy(out, tmpData2, flen*2);
  
	RSA_free(r);
	return 0;
}