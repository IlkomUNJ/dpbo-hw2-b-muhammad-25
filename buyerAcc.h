#ifndef BUYERACC_H
#define BUYERACC_H

#include <string>
#include "userBankAcc.h"
#include "sellerAcc.h"
#include "product.h"
#include "orders.h"
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

class buyerAcc{
private:
    string name;
    string password;
    int statusLogin = 0;
    userBankAcc bankAcc;
    vector <order> orderan; 
    bool toko;
    sellerAcc selAcc;

public:
    buyerAcc() : name(""), password(""), statusLogin(0), toko(false) {}
    buyerAcc(string name1, string password1, bool toko1)
        : name(name1), password(password1), toko(toko1) {}

    string getPassword () const { return password; }
    string getName () const { return name; }
    double getSaldo () const { return bankAcc.getSaldo(); }
    string getPin () const { return bankAcc.getPin(); }

    void bikinBankAcc (string pin,double saldo) {
        bankAcc = userBankAcc(pin,saldo);
    }

    void bikinSellerAcc (string namaToko, string address1) {
        selAcc = sellerAcc(namaToko,address1);
        toko = true;
    }

    void addsaldo (double saldo) { bankAcc.addSaldo(saldo); }
    bool withdraw (double saldo) { return bankAcc.withdraw(saldo); }

    void tampilinSaldo () {
        cout << "\n-------------------------------------------------------------\n";
        cout << "Account Owner" << ": " << name << "\n";
        cout << "Current Balance" << ": Rp " << bankAcc.getSaldo() << "\n";
        cout << "\n-------------------------------------------------------------\n";
    }

    
    void tambahTransaksi (double saldo1, bool kondisi) { bankAcc.addTransaksi(saldo1, kondisi); }

    void tampilinTransaksi () { bankAcc.tampilinTransaksi(); }

    bool is_seller () const { return toko; }

    const userBankAcc& getBankAcc() const { return bankAcc; }
    userBankAcc& getBankAccRef() { return bankAcc; }

    string getStoreName() const { return selAcc.getStoreName(); }
    string getStoreAddress() const { return selAcc.getAddress(); }

    void addproduct (string skuid) {selAcc.addProduct(skuid);}

    void display_products() {selAcc.display_products();}

    int getJumlahProduk () const {return selAcc.getJumlahProduk();}

    void edit_produk (int indeks,int stok, double harga) {selAcc.edit_produk(indeks,stok,harga);}
}; 

extern vector<buyerAcc> buyers;

#endif
