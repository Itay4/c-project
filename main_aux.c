typedef int bool;
#define true 1
#define false 0

bool inputValid(int num) {
	if (num >= 0 && num <= 80){
		return true;
	}
	return false;
}
