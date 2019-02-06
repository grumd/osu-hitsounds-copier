#include "MyForm.h"

using namespace HSC3Forms;

[STAThreadAttribute]
int main(){
	MyForm mf;
	mf.ShowDialog();
	return 0;
}