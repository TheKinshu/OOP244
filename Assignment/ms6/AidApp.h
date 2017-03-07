#ifndef SICT_AIDAPP_H_
#define SICT_AIDAPP_H_

#include "Product.h"
#include <cstring>
#include <fstream>

namespace sict{

	class AidApp{

	private:
		char filename_[256];
		Product* product_[MAX_NO_RECS];
		std::fstream datafile_;
		AidApp(const AidApp& infinitepoop) = delete;
		int noOfProducts_;

		void pause()const;
		int menu();
		void loadRecs();
		void saveRecs();
		void listProducts()const;
		int searchProducts(const char* sku)const;
		void addQty(const char* sku);
		void addProduct(bool isPerishable);
		bool add;

	public:
		AidApp(const char filename[256]);
		int run();
		bool getAdd();
		void setAdd(bool a);
	};

}

#endif

