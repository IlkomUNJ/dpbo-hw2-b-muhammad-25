#ifndef SELLERACC_H
#define SELLERACC_H

#include <string>
#include <iomanip>
#include <iostream>
#include "product.h"


using namespace std;

class sellerAcc {
private:
    string address;
    string storeName;
    vector <string> skuProduct;

public:
    sellerAcc(){};
    sellerAcc(string storeName1, string address1) 
        : storeName(storeName1), address(address1) {};

    string getStoreName() const { return storeName; }
    string getAddress() const { return address; }
    
    void addProduct(string sku) {skuProduct.push_back(sku);}


    product* getProductBySku (const string &sku) {
        for (auto &p : allProduct ) {
            if (sku == p.getSku()) {
                return &p; 
            }
        }
        return nullptr;
    }

    int getJumlahProduk () const {return skuProduct.size();}

    void edit_produk (int indeks,int stok, double harga) {
        if (indeks <= 0 || indeks > (int)skuProduct.size()) {
            cout << "Index produk tidak valid.\n";
            return;
        }
        string sku = skuProduct[indeks-1];
        product* produk = getProductBySku(sku);
        if (produk) {
            produk->editProduk(stok, harga);
            cout << "Produk dengan SKU " << sku << " berhasil diubah.\n";
        } else {
            cout << "Produk dengan SKU " << sku << " tidak ditemukan di allProduct.\n";
        }
    }

    void display_products() { 
        int no = 1;
        cout << "\n================================================\n";
        cout << "              LIST YOUR PRODUCT STORE             \n";
        cout << "==================================================\n";
        cout << left << setw(5)  << "No."
            << setw(20) << "Nama Produk"
            << setw(10) << "Harga"
            << setw(10) << "Stok"
            << setw(10) << "SKU ID"<< endl;
        cout <<"--------------------------------------------------\n";

        for (const auto& psku : skuProduct ) {
            product* produk = getProductBySku(psku);
            if (produk) {
                cout << left << setw(5)  << no
                    << setw(20) << produk->getName()
                    << "Rp " << setw(8) << fixed << setprecision(0) << produk->getPrice()
                    << setw(5)  << produk->getJumlah()
                    << setw(10) << produk->getSku() << endl;
            } else {
                cout << left << setw(5) << no << setw(20) << "(produk tidak ditemukan)" << setw(10) << "" << setw(10) << "" << psku << endl;
            }
            ++no;
        }
        cout << "=================================================\n";
    }
    
};



#endif
