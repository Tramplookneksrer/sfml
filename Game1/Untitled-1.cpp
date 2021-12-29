#include <iostream>
#include <ctime>
using namespace std;

struct point {
	double x;
	double y;
	double z;
};

struct sphere {
	double x;
	double y;
	double z;
	double R;
};

double random(double, double);
bool hit(sphere&, point&);
int shoot(sphere&, int, double, double);

int main() {
	srand(time(0));
	double low, upper;
	cout << "enter low & upper limits: ";
	cin >> low >> upper;
	if (low > upper) {
		swap(low, upper);
	}
	sphere s;
	cout << "enter sphere center (x,y,z): ";
	cin >> s.x >> s.y >> s.z;
	cout << "enter sphere radius R: ";
	cin >> s.R;
	int attempts;
	cout << "enter number of attempts: ";
	cin >> attempts;
	cout << shoot(s, attempts, low, upper) << " hits.\n";
	return 0;
}

double random(double low, double upper) {
	return low + rand() % int(upper - low + 1);
}

bool hit(sphere& s, point& p) {
	return (p.x - s.x) * (p.x - s.x) + (p.y - s.y) * (p.y - s.y) + (p.z - s.z) * (p.z - s.z) < s.R * s.R;
}

int shoot(sphere& s, int attempts, double low, double upper) {
	int count = 0;
	point p;
	for (int i = 0; i < attempts; i++) {
		p.x = random(low, upper);
		p.y = random(low, upper);
		p.z = random(low, upper);
		if (hit(s, p)) {
			count++;
		}
	}
	return count;
}
