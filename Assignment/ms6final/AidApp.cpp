
#include "AidApp.h"
#include "AmaProduct.h"
#include "AmaPerishable.h"
#include <iomanip>
#include <cstring>
#include <fstream>

using namespace std;
namespace sict{

	void AidApp::pause()const{
		cin.ignore(1000, '\n');
		cout << "Press Enter to continue..." << endl;
	}
	int AidApp::menu(){

		int menunum;

		cout << "Disaster Aid Supply Management Program" << endl;
		cout << "1- List products" << endl;
		cout << "2- Display product" << endl;
		cout << "3- Add non-perishable product" << endl;
		cout << "4- Add perishable product" << endl;
		cout << "5- Add to quantity of purchased products" << endl;
		cout << "0- Exit program" << endl;
		cout << "> ";

		cin >> menunum;

		if (menunum == 1 || menunum == 2 || menunum == 3 || menunum == 4 || menunum == 5 || menunum == 0){
			return menunum;
		}
		else {
			pause();
			return -1;
		}
	}

	void AidApp::loadRecs() {

		int i = 0;
		char prdSku[20];

		datafile_.open(filename_, ios::in);

		if (datafile_.is_open()) {
			while (datafile_.good()) {
				datafile_.getline(prdSku, 10, ',');
				if (prdSku[0] == 'N') {
					product_[i] = new AmaProduct;
					product_[i]->load(datafile_);
					if (!datafile_.eof()){
						datafile_.getline(prdSku, 10, '\n');
					}
					i++;
				}
				else if (prdSku[0] == 'P') {
					product_[i] = new AmaPerishable;
					product_[i]->load(datafile_);
					if (!datafile_.eof()){
						datafile_.getline(prdSku, 10, '\n');
					}
					i++;
				}
				prdSku[0] = 0;
			}
		}
		else {
			datafile_.clear();
			datafile_.close();
			datafile_.open("aidapp.txt", ios::out);
			datafile_.close();
		}
		noOfProducts_ = i;
		datafile_.close();
	}

	void AidApp::saveRecs() {
		if (getAdd()){
			noOfProducts_++;
		}
		datafile_.open(filename_, ios::out);
		for (int i = 0; i < noOfProducts_; i++) {
			product_[i]->store(datafile_, true);
		}
		product_[noOfProducts_] = nullptr;
		datafile_.clear();
		datafile_.close();
	}

	void AidApp::listProducts()const{
		cin.ignore(1000, '\n');
		cin.ignore(1000, '\n');
		double totalcost = 0;
		double tempCost = 0;
		double tempQty = 0;
		cout << " Row | SKU    | Product Name       | Cost  | QTY| Unit     |Need| Expiry   " << endl;
		cout << "-----|--------|--------------------|-------|----|----------|----|----------" << endl;
		for (int i = 0; i < noOfProducts_; i++){
			cout.fill(' ');
			cout << setw(4) << right << (i + 1) << " | ";
			product_[i]->write(cout, true);
			tempCost = product_[i]->cost();
			tempQty = product_[i]->quantity();
			cout << setprecision(2);
			totalcost += tempCost * tempQty;
			if (((i+1 == 10) && (i > 0))){
				pause();
				cin.ignore(1000, '\n');
			}
			else{
				cout << endl;
			}
		}
		cout << "---------------------------------------------------------------------------" << endl;
		cout << "Total cost of support: $" << setprecision(2) << totalcost << endl;
		pause();
		cin.ignore();
	}

	int AidApp::searchProducts(const char* sku)const{
		int skuno = 0;
		char temp[200];
		for (int i = 0; i < noOfProducts_; i++){
			strcpy(temp, product_[i]->sku());
			if (strcmp(temp, sku) == 0){
				skuno = i;
				i = noOfProducts_ + 1;
			}
			else{
				skuno = -1;
			}
		}
		temp[0] = 0;
		return skuno;
	}

	void AidApp::addQty(const char* sku){
		int quantity = 0;
		bool is_fail = true;
		char temp[200];
		for (int i = 0; i < noOfProducts_; i++){
			strcpy(temp, product_[i]->sku());
			if (strcmp(temp, sku) == 0){
				is_fail = false;
				product_[i]->write(cout, false);
				cin.ignore(1000, '\n');
				cout << endl << "Please enter the number of purchased items: ";
				cin >> quantity;
				if (quantity){
					if (quantity <= product_[i]->qtyNeeded() - product_[i]->quantity()){
						quantity += product_[i]->quantity();
						product_[i]->quantity(quantity);
						saveRecs();
						cin.ignore(1000, '\n');
						cin.ignore(1000, '\n');
						cout << "Updated!" << endl;
						cin.ignore(1000, '\n');
					}
					else{
						cout << "Too many items; only " << product_[i]->qtyNeeded() - product_[i]->quantity() << " is needed, please return the extra "
							<< (product_[i]->quantity() + quantity) - product_[i]->qtyNeeded() << " items." << endl;
						cin.ignore(1000, '\n');
						cin.ignore(1000, '\n');
						cout << "Updated!" << endl;
						cin.ignore(1000, '\n');
					}
				}
				else{
					cout << "Invalid quantity value!" << endl;
				}
			}
		}
		temp[0] = 0;
		if (is_fail){
			cout << "Not found!" << endl;
		}
	}

	void AidApp::addProduct(bool isPerishable){
		cin.ignore(1000, '\n');
		cin.ignore(1000, '\n');
		if (isPerishable){
			product_[noOfProducts_] = new AmaPerishable;
		}
		else{
			product_[noOfProducts_] = new AmaProduct;
		}
		product_[noOfProducts_]->read(cin);
	}

	AidApp::AidApp(const char filename[256]){
		strcpy(filename_, filename);
		noOfProducts_ = 0;
		product_[0] = nullptr;
		loadRecs();
		setAdd(false);
	}

	int AidApp::run(){
		int options = menu();
		char sku[MAX_SKU_LEN];
		int skuno;
		bool getAdd();
		if (product_[0] == nullptr){
			loadRecs();
		}

		if (options == 1){
			listProducts();
			run();
		}

		else if (options == 2){
			cin.ignore(1000, '\n');
			cin.ignore(1000, '\n');
			cout << "Please enter the SKU: ";
			cin >> sku;
			cin.ignore(1000, '\n');
			cin.ignore(1000, '\n');
			skuno = searchProducts(sku);

			if (skuno == -1){

				cout << "Not found!" << endl;
				pause();
				run();
			}
			else{
				product_[skuno]->write(cout, false);
				cin.ignore(1000, '\n');
				pause();
				cin.ignore(1000, '\n');
				run();
			}
		}

		else if (options == 3){
			setAdd(true);
			addProduct(false);
			saveRecs();
			cin.ignore(1000, '\n');
			cin.ignore(1000, '\n');
			run();
		}

		else if (options == 4){
			setAdd(true);
			addProduct(true);
			saveRecs();
			cin.ignore(1000, '\n');
			cin.ignore(1000, '\n');
			cout << "Product added" << endl;
			cin.ignore(1000, '\n');
			run();
		}
		else if (options == 5){
			cin.ignore(1000, '\n');
			cin.ignore(1000, '\n');
			cout << "Please enter the SKU: ";
			cin >> sku;
			cin.ignore(1000, '\n');
			cin.ignore(1000, '\n');
			skuno = searchProducts(sku);
			if (skuno == -1){
				addQty(sku);
				cout << "Not found!" << endl;
			}
			else{
				setAdd(false);
				addQty(sku);
			}
			sku[0] = 0;
			run();

		}

		if (options == 0){
			cin.ignore(1000, '\n');
			cin.ignore(1000, '\n');
			cout << "Goodbye!!" << endl;
		}

		if (options > 5){
			cout << "===Invalid Selection, try again.===" << endl;
			pause();
			run();
		}

		return 0;
	}

	void AidApp::setAdd(bool a){
		add = a;
	}

	bool AidApp::getAdd(){
		return add;
	}
}

