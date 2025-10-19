// storageCSV.h
#ifndef STORAGECSV_H
#define STORAGECSV_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "buyerAcc.h"
#include "product.h"
#include "orders.h"

using namespace std;

static const string BUYERS_CSV = "buyers.csv";
static const string TRANSACTIONS_CSV = "transactions.csv";
static const string PRODUCTS_CSV = "product.csv";
static const string ORDERS_CSV = "orders.csv";

static string escapeCSV(const string& field) {
    bool needQuotes = false;
    for (char c : field) {
        if (c == ',' || c == '"' || c == '\n' || c == '\r') { needQuotes = true; break; }
    }
    if (!needQuotes) return field;
    string out = "\"";
    for (char c : field) {
        if (c == '"') out += "\"\"";
        else out += c;
    }
    out += "\"";
    return out;
}

static string unescapeCSV(const string& field) {
    if (field.size() >= 2 && field.front() == '"' && field.back() == '"') {
        string inner = field.substr(1, field.size() - 2);
        string out;
        for (size_t i = 0; i < inner.size(); ++i) {
            if (inner[i] == '"' && i + 1 < inner.size() && inner[i+1] == '"') {
                out.push_back('"'); i++;
            } else out.push_back(inner[i]);
        }
        return out;
    }
    return field;
}

static vector<string> parseCSVLine(const string& line) {
    vector<string> result;
    string cur;
    enum State {NORMAL, IN_QUOTE} state = NORMAL;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (state == NORMAL) {
            if (c == ',') {
                result.push_back(cur);
                cur.clear();
            } else if (c == '"') {
                state = IN_QUOTE;
            } else cur.push_back(c);
        } else { 
            if (c == '"') {
                if (i + 1 < line.size() && line[i+1] == '"') {
                    cur.push_back('"'); ++i; 
                } else {
                    state = NORMAL; 
                }
            } else cur.push_back(c);
        }
    }
    result.push_back(cur);
    for (auto &s : result) {
        if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
            s = unescapeCSV(s);
        }
    }
    return result;
}

inline bool saveBuyersToCSV(const string& filename = BUYERS_CSV) {
    ofstream ofs(filename, ios::trunc);
    if (!ofs.is_open()) return false;

    ofs << "name,password,pin,saldo,toko,storeName,address\n";
    for (const auto& b : buyers) {
        string name = b.getName();
        string pass = b.getPassword();
        string pin = b.getPin();
        double saldo = b.getSaldo();
        bool toko = b.is_seller();
        string storeName = toko ? b.getStoreName() : "";
        string address = toko ? b.getStoreAddress() : "";

        ofs << escapeCSV(name) << ','
            << escapeCSV(pass) << ','
            << escapeCSV(pin) << ','
            << saldo << ','
            << (toko ? "1" : "0") << ','
            << escapeCSV(storeName) << ','
            << escapeCSV(address)
            << '\n';
    }
    ofs.close();
    return true;
}


inline bool saveTransactionsToCSV(const string& filename = TRANSACTIONS_CSV) {
    ofstream ofs(filename, ios::trunc);
    if (!ofs.is_open()) return false;

    ofs << "ownerName,amount,date\n";
    for (const auto& b : buyers) {
        const string owner = b.getName();
        const auto& trans = b.getBankAcc().getTransaksi();
        for (const auto& t : trans) {
            double amount = t.getAmount(); 
            string date = t.getDate();
            ofs << escapeCSV(owner) << ','
                << amount << ','
                << escapeCSV(date) << '\n';
        }
    }
    ofs.close();
    return true;
}

// ----- SAVE PRODUCTS -----
inline bool saveProductsToCSV(const string& filename = PRODUCTS_CSV) {
    ofstream ofs(filename, ios::trunc);
    if (!ofs.is_open()) return false;

    ofs << "sku,name,sellerName,jumlah,price\n";
    for (const auto& p : allProduct) {
        ofs << escapeCSV(p.getSku()) << ','
            << escapeCSV(p.getName()) << ','
            << escapeCSV(p.getSellerName()) << ','
            << p.getJumlah() << ','
            << p.getPrice() << '\n';
    }
    ofs.close();
    return true;
}

// ----- SAVE ORDERS -----
inline bool saveOrdersToCSV(const string& filename = ORDERS_CSV) {
    ofstream ofs(filename, ios::trunc);
    if (!ofs.is_open()) return false;

    ofs << "sku,buyerName,nameProduct,sellerName,harga,date,jumlah,amount\n";
    for (const auto& o : allOrders) {
        ofs << escapeCSV(o.getSku()) << ','
            << escapeCSV(o.getbuyNAme()) << ','
            << escapeCSV(o.getName()) << ','
            << escapeCSV(o.getSellerName()) << ','
            << o.getHarga() << ','
            << escapeCSV(o.getDate()) << ','
            << o.getJumlah() << ','
            << o.getamount() << '\n';
    }
    ofs.close();
    return true;
}

// Save all data files (now includes orders)
inline bool saveAllDataCSV(const string& buyersFile = BUYERS_CSV, const string& transFile = TRANSACTIONS_CSV, const string& prodFile = PRODUCTS_CSV, const string& ordersFile = ORDERS_CSV) {
    bool a = saveBuyersToCSV(buyersFile);
    bool b = saveTransactionsToCSV(transFile);
    bool c = saveProductsToCSV(prodFile);
    bool d = saveOrdersToCSV(ordersFile);
    return a && b && c && d;
}

// ----- LOAD BUYERS -----
inline bool loadBuyersFromCSV(const string& filename = BUYERS_CSV) {
    ifstream ifs(filename);
    if (!ifs.is_open()) {
        return true;
    }
    string line;
    buyers.clear();

    if (!getline(ifs, line)) return true;

    while (getline(ifs, line)) {
        if (line.size() == 0) continue;
        auto fields = parseCSVLine(line);
        string name = fields.size() > 0 ? unescapeCSV(fields[0]) : "";
        string pass = fields.size() > 1 ? unescapeCSV(fields[1]) : "";
        string pin  = fields.size() > 2 ? unescapeCSV(fields[2]) : "";
        double saldo = 0.0;
        if (fields.size() > 3) {
            try { saldo = stod(fields[3]); } catch(...) { saldo = 0.0; }
        }
        bool toko = false;
        if (fields.size() > 4) {
            string t = fields[4];
            if (t == "1" || t == "true" || t == "True") toko = true;
        }
        string storeName = (fields.size() > 5 ? unescapeCSV(fields[5]) : "");
        string address = (fields.size() > 6 ? unescapeCSV(fields[6]) : "");

        buyers.push_back(buyerAcc(name, pass, toko));
        buyers.back().getBankAccRef().setPinSaldo(pin, saldo);
        if (toko && storeName.size() > 0) {
            buyers.back().bikinSellerAcc(storeName, address);
        }
    }
    ifs.close();
    return true;
}

// ----- LOAD TRANSACTIONS -----
inline bool loadTransactionsFromCSV(const string& filename = TRANSACTIONS_CSV) {
    ifstream ifs(filename);
    if (!ifs.is_open()) {
        return true;
    }
    string line;
    if (!getline(ifs, line)) return true;

    while (getline(ifs, line)) {
        if (line.size() == 0) continue;
        auto fields = parseCSVLine(line);
        if (fields.size() < 2) continue;
        string owner = unescapeCSV(fields[0]);
        double amount = 0.0;
        try { amount = stod(fields[1]); } catch(...) { amount = 0.0; }
        string date = (fields.size() > 2 ? unescapeCSV(fields[2]) : "");

        auto it = find_if(buyers.begin(), buyers.end(), [&](const buyerAcc& b) {
            return b.getName() == owner;
        });
        if (it != buyers.end()) {
            if (amount >= 0) {
                // menjaga tanggal asli (date) jika ada
                it->getBankAccRef().addTransaksi(amount, true, date);
            } else {
                it->getBankAccRef().addTransaksi(fabs(amount), false, date);
            }
        }
    }

    ifs.close();
    return true;
}


// ----- LOAD PRODUCTS -----
inline bool loadProductsFromCSV(const string& filename = PRODUCTS_CSV) {
    ifstream ifs(filename);
    if (!ifs.is_open()) {
        return true;
    }
    string line;
    if (!getline(ifs, line)) return true;

    allProduct.clear();

    while (getline(ifs, line)) {
        if (line.size() == 0) continue;
        auto fields = parseCSVLine(line);
        if (fields.size() < 4) continue;
        string sku = unescapeCSV(fields[0]);
        string name = fields.size() > 1 ? unescapeCSV(fields[1]) : "";
        string seller = fields.size() > 2 ? unescapeCSV(fields[2]) : "";
        int jumlah = 0;
        double price = 0.0;
        try { jumlah = stoi(fields[3]); } catch(...) { jumlah = 0; }
        if (fields.size() > 4) {
            try { price = stod(fields[4]); } catch(...) { price = 0.0; }
        }

        allProduct.push_back(product(sku, name, seller, jumlah, price));
    }

    ifs.close();

    for (const auto &p : allProduct) {
        string seller = p.getSellerName();
        string sku = p.getSku();
        for (auto &b : buyers) {
            if (b.is_seller() && b.getStoreName() == seller) {
                b.addproduct(sku);
            }
        }
    }

    return true;
}

// ----- LOAD ORDERS -----
inline bool loadOrdersFromCSV(const string& filename = ORDERS_CSV) {
    ifstream ifs(filename);
    if (!ifs.is_open()) {
        return true;
    }
    string line;
    if (!getline(ifs, line)) return true;

    allOrders.clear();

    while (getline(ifs, line)) {
        if (line.size() == 0) continue;
        auto fields = parseCSVLine(line);
        if (fields.size() < 8) continue;
        string sku = unescapeCSV(fields[0]);
        string buyer = unescapeCSV(fields[1]);
        string nameProd = unescapeCSV(fields[2]);
        string seller = unescapeCSV(fields[3]);
        double harga = 0.0;
        try { harga = stod(fields[4]); } catch(...) { harga = 0.0; }
        string date = unescapeCSV(fields[5]);
        int jumlah = 0;
        try { jumlah = stoi(fields[6]); } catch(...) { jumlah = 0; }
        double amount = 0.0;
        try { amount = stod(fields[7]); } catch(...) { amount = 0.0; }

        allOrders.emplace_back(sku, nameProd, buyer, seller, harga, date, jumlah, amount);
    }

    ifs.close();
    return true;
}

// Load both files
inline bool loadAllDataCSV(const string& buyersFile = BUYERS_CSV, const string& transFile = TRANSACTIONS_CSV, const string& prodFile = PRODUCTS_CSV, const string& ordersFile = ORDERS_CSV) {
    bool a = loadBuyersFromCSV(buyersFile);
    bool b = loadTransactionsFromCSV(transFile);
    bool c = loadProductsFromCSV(prodFile);
    bool d = loadOrdersFromCSV(ordersFile);
    return a && b && c && d;
}

#endif
