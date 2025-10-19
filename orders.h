// orders.h
#ifndef ORDERS_H
#define ORDERS_H
#include <string>
#include <vector>
#include "product.h"
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

static string currentTanggalString() {
    time_t t = time(nullptr);
    tm local_tm;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&local_tm, &t);
#else
    localtime_r(&t, &local_tm);
#endif
    std::ostringstream oss;
    oss << setw(2) << setfill('0') << local_tm.tm_mday << "/"
        << setw(2) << setfill('0') << (local_tm.tm_mon + 1) << "/"
        << (local_tm.tm_year + 1900);
    return oss.str();
}

class order{
private:
    string skuID;
    string buyerName;
    string nameProduct;
    string sellerName;
    double harga;
    string date;
    int jumlah;
    double amount;

public:
    order() : skuID(""), buyerName(""), nameProduct(""), sellerName(""), harga(0.0), date(currentTanggalString()), jumlah(0), amount(0.0) {}
    order (product produk, string buyNAme, double amount1, int jumlah1) 
        : buyerName(buyNAme),
          nameProduct(produk.getName()),
          harga(produk.getPrice()),
          jumlah(jumlah1),
          amount(amount1),
          skuID(produk.getSku()),
          sellerName(produk.getSellerName()),
          date(currentTanggalString()) {};

    order (const string& sku,
           const string& nameProd,
           const string& buyName,
           const string& seller,
           double harga1,
           const string& date1,
           int jumlah1,
           double amount1)
        : skuID(sku),
          buyerName(buyName),
          nameProduct(nameProd),
          sellerName(seller),
          harga(harga1),
          date(date1),
          jumlah(jumlah1),
          amount(amount1) {}

    string getName() const {return nameProduct;}
    string getSku() const {return skuID;}
    string getbuyNAme () const {return buyerName;}
    string getSellerName() const {return sellerName;}
    int getJumlah () const{return jumlah;}
    double getamount () const{return amount;}
    double getHarga () const{return harga;}
    string getDate() const { return date; }
};

extern vector <order> allOrders;

#endif
