
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

		int menuvalue;

		cout << "Disaster Aid Supply Management Program" << endl;
		cout << "1- List products" << endl;
		cout << "2- Display product" << endl;
		cout << "3- Add non-perishable product" << endl;
		cout << "4- Add perishable product" << endl;
		cout << "5- Add to quantity of purchased products" << endl;
		cout << "0- Exit program" << endl;
		cout << "> ";

		cin >> menuvalue;

		if (menuvalue == 1 || menuvalue == 2 || menuvalue == 3 || menuvalue == 4 || menuvalue == 5 ||menuvalue == 0){
			return menuvalue;
		}
		else {
			pause();
			return -1;
		}
	}

	void AidApp::loadRecs() {

		int i = 0;
		char type[20];

		datafile_.open(filename_, ios::in);

		if (datafile_.is_open()) {
			while (datafile_.good()) {
				datafile_.getline(type, 10, ',');
				if (type[0] == 'N') {
					product_[i] = new AmaProduct;
					product_[i]->load(datafile_);
					if (!datafile_.eof()){
						datafile_.getline(type, 10, '\n');
					}
					i++;
				}
				else if (type[0] == 'P') {
					product_[i] = new AmaPerishable;
					product_[i]->load(datafile_);
					if (!datafile_.eof()){
						datafile_.getline(type, 10, '\n');
					}
					i++;
				}
				type[0] = '\0';
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
		if (getAdd()) {
			noOfProducts_++;
		}
		datafile_.open(filename_, ios::out);
		for (int i = 0; i < noOfProducts_; i++) {
			product_[i]->store(datafile_, true);
		}
		datafile_.clear();
		datafile_.setstate(ios::failbit);

		strcpy(filename_, "");
		noOfProducts_ = 0;
		product_[0] = nullptr;
		loadRecs();
		setAdd(false);
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
			if (((i % 10) == 0) && (i > 0))
				pause();
			cout.fill(' ');
			cout << setw(4) << right << (i + 1) << " | ";
			product_[i]->write(cout, true);
			tempCost = product_[i]->cost();
			tempQty = product_[i]->quantity();
			cout << setprecision(2);
			totalcost += tempCost * tempQty;
			cout << endl;
		}
		cout << "---------------------------------------------------------------------------" << endl;
		cout << "Total cost of support: $" << setprecision(2) << totalcost << endl;
		pause();
		cin.ignore();
	}

	int AidApp::searchProducts(const char* sku)const{

		int skuno = 0;

		for (int i = 0; i < noOfProducts_; i++){
			if (strcmp(product_[i]->sku(), sku) == 0){
				skuno = i;
				i = noOfProducts_ + 1;
			}
			else{
				skuno = -1;
			}
		}
		return skuno;
	}

	void AidApp::addQty(const char* sku){

		int quantity = 0;
		bool fail = true;
		char temp[200];
		for (int i = 0; i < noOfProducts_; i++){
			strcpy(temp, product_[i]->sku());
			if (strcmp(temp, sku) == 0){
				fail = false;
				product_[i]->write(cout, false);
				cout << endl << "Please enter the number of purchased items: ";
				cin >> quantity;
				if (quantity){
					if (quantity <= product_[i]->qtyNeeded() - product_[i]->quantity()){
						quantity += product_[i]->quantity();
						product_[i]->quantity(quantity);
						saveRecs();
						cout << "Updated!" << endl;
					}
					else{
						quantity = product_[i]->qtyNeeded();
						cout << "Too many items; only" << product_[i]->qtyNeeded() << "is needed, please return the extra 99 items." << endl;
					}
				}
				else{
					cout << "Invalid quantity value!" << endl;
				}
			}
		}
		if (fail){
			cout << "Not found!" << endl;
		}
	}

	void AidApp::addProduct(bool isPerishable){
		cin.ignore(1000, '\n');
		cin.ignore(1000, '\n');
		if (isPerishable){
			product_[noOfProducts_] = new AmaPerishable;
			product_[noOfProducts_]->read(cin);
		}
		else{
			product_[noOfProducts_] = new AmaProduct;
			product_[noOfProducts_]->read(cin);
		}
	}

	AidApp::AidApp(const char filename[256]){

		strcpy(filename_, filename);
		noOfProducts_ = 0;
		product_[0] = nullptr;
		loadRecs();
		setAdd(false);
	}

	int AidApp::run(){

		int option = menu();
		char sku[MAX_SKU_LEN];
		int skuno;
		bool getAdd();

		if (product_[0] == nullptr){
			loadRecs();
		}

		if (option == 1){
			listProducts();
			run();
		}

		if (option == 2){
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

		if (option == 3){
			setAdd(true);
			addProduct(false);
			saveRecs();
			run();
		}

		if (option == 4){
			setAdd(true);
			addProduct(true);
			saveRecs();
			run();
		}
		if (option == 5){
			cout << "Please enter the SKU: ";
			cin >> sku;
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
			pause();
			run();

		}

		if (option != 1 && option != 2 && option != 3 && option != 4 && option != 5 && option != 0){
			cout << "===Invalid Selection, try again.===" << endl;
			pause();
			run();
		}

		if (option == 0){
			cout << "Goodbye!!";
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

