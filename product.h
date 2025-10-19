#ifndef PRODUCT_H
#define PRODUCT_H
#include <string>
#include <vector>

using namespace std;

class product{
private:
    string skuID;
    string nameProduct;
    string sellerName;
    int jumlah;
    double price;

public:
    product() {};
    product (string skuId, string nama, string sellerName1 ,int jumlah1, double price1) 
        : nameProduct(nama), jumlah(jumlah1), price(price1), skuID(skuId), sellerName(sellerName1) {};
    
    string getName() const {return nameProduct;}
    string getSku() const {return skuID;}
    string getSellerName() const {return sellerName;}
    int getJumlah () const{return jumlah;}
    double getPrice () const{return price;}

    void editProduk (int jum, double pric) {
       jumlah = jum;
       price = pric;
    }

    void decProduct (int jumlah1) {
        jumlah -= jumlah1;
    }
};

extern vector<product> allProduct;

#endif
