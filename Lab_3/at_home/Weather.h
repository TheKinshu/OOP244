// compilation safegaurds
#define NUMBEROFELEMENTS 3
#

/*Weather.h*/
namespace sict{
	// sict namespace
	class Weather{
		// member variables
		char _date[8];
		double _highTemp;
		double _lowTemp;
	public:
		// member function
		void set(const char* date, double hTemp, double lTemp);
		void display() const;
		const char* date() const;
		double low() const;
	};
}
