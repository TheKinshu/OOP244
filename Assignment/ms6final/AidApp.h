#ifndef SICT_AIDAPP_H_
#define SICT_AIDAPP_H_

#include "Product.h"
#include <cstring>
#include <fstream>

namespace sict{

	class AidApp{

	private:
		//variables
		char filename_[256];
		int noOfProducts_;
		Product* product_[MAX_NO_RECS];

		std::fstream datafile_;
		AidApp(const AidApp& a) = delete;
		
		//funtions
		void listProducts()const;
		int searchProducts(const char* sku)const;
		void addQty(const char* sku);
		void loadRecs();
		void saveRecs();
		void addProduct(bool isPerishable);
		bool add;
		void pause()const;
		int menu();

	public:
		//constructor
		AidApp(const char filename[256]);
		int run();
		bool getAdd();
		void setAdd(bool a);
	};

}

#endif

