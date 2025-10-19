// userBankAcc.h
#ifndef USERBANKACC_H
#define USERBANKACC_H

#include <string>
#include <vector>
#include <cmath>
#include "transaksi.h"

using namespace std;

class userBankAcc{
private:
    string pin;
    double saldo;
    vector<transaksi> trasaction;

public:
    userBankAcc(){}
    userBankAcc(string pin1, double saldo1) 
        : pin(pin1), saldo(saldo1) {
            addTransaksi(saldo1,true);
        };
    
    void addSaldo (double tambah) { saldo +=  tambah; }
    bool withdraw (double kurang) {
        if(saldo < kurang) return false;
        else {
            saldo -=  kurang;
            return true;
        }
    }
    double getSaldo () const { return saldo; }
    string getPin () const { return pin; }

    void addTransaksi (double saldo1, bool kondisi, const string& date = "") {
        if(kondisi) {
            trasaction.push_back(transaksi(saldo1, date));
        }else {
            trasaction.push_back(transaksi((saldo1 * -1 ), date));
        }
    }

    void tampilinTransaksi () {
        string kondisi;
        for ( int i = 0; i < trasaction.size() ; i++){
            if (trasaction[i].getAmount() >= 0 ) kondisi = "Balance In";
            else kondisi = "Balance Out";
            cout << "-------------------------------------------------------------\n";
            cout << "Transaction ID : " << kondisi << "\n";
            cout << "Amount         : Rp " << fixed << setprecision(2) << trasaction[i].getAmount() << "\n";
            cout << "Date           : " << trasaction[i].getDate() << "\n";
        }
    }

    const vector<transaksi>& getTransaksi() const { return trasaction; }
    vector<transaksi>& getTransaksiRef() { return trasaction; }
    void setPinSaldo(const string& p, double s) { pin = p; saldo = s; }
};

#endif
