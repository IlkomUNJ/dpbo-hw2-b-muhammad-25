// main.cpp
#include <iostream>
#include <string>
#include <iomanip>
#include "buyerAcc.h"
#include "userBankAcc.h"
#include "storageCSV.h"
#include "product.h"
#include "orders.h"
#include <vector>
#include <limits>

enum main_prompt{LOGIN, REGISTER, EXIT, MAIN_MENU};
enum buyer_menu_prompt {PRODUCT_LIST,ORDER_LIST,TO_BANK,UPSELLER, BALIK ,BUYER_MENU};
enum seller_menu_prompt {PRODUCT_LIST1,ORDER_LIST1,TO_BANK1, SELLER_MENU,BALIK2 ,BUYER_MENU1};
enum bank_fitur_prompt {CEK_SALDO, TOP_UP,WITHDRAW,CASH_FLOW, BALIK1,BANK_FITUR_MENU};
enum RegisterMenu { AS_BUYER, AS_SELLER, EXIT3, MENU1};
enum loginMenu { LOG_BUYER, LOG_SELLER, EXIT4, MENU2};
enum menu_toko {LIST_PRODUK, UPLOAD_PRODUK, EDIT_PRODUK, PESANAN,EXIT_MENU4, TOKO_MENU};

using namespace std;

vector<buyerAcc> buyers;
vector <product> allProduct;
vector <order> allOrders;

int current_buyer_index = -1;

inline bool hasLoggedIn() {
    return current_buyer_index >= 0 && current_buyer_index < (int)buyers.size();
}

double regSaldo () {
    string input;
    double value;
    while (true) {
        cout << "Balance (exp : 50000)  : RP.  ";
        cin.clear();
        getline(cin >> ws, input);

        try {
            for (auto &c : input) if (c == ',') c = '.';

            size_t pos;
            value = stod(input, &pos);

            if (pos != input.length()) {
                throw invalid_argument("Extra characters found");
            }
            break;
        } 
        catch (const invalid_argument&) {
            cout << "Invalid input. Please enter a valid number.\n";
        } 
        catch (const out_of_range&) {
            cout << "Number is too large. Try a smaller one.\n";
        }
    }
    return value;
}

bool cek_duplicate_name (string name) {
    for (int  i = 0; i < buyers.size(); i++){
        if(name == buyers[i].getName()) return false;
    }
    return true;
}

bool cek_duplicate_store_name (string name) {
    for (int  i = 0; i < buyers.size(); i++){
        if(name == buyers[i].getStoreName()) return false;
    }
    return true;
}

bool Login_verif(string name, string password){
    for(size_t i = 0; i < buyers.size(); ++i){
        if(buyers[i].getName() == name && buyers[i].getPassword() == password){
            current_buyer_index = (int)i;
            return true;
        }
    }
    return false;
}

void buatToko () {
    string store,alamat;
    cout << "----------------------------------------------\n";
    cout << "now create your shop account\n";
    cout << "----------------------------------------------\n\n";
    while (true){
        cout << "Store Name   : ";
        getline(cin >> ws, store);

        if(cek_duplicate_store_name(store)) break;
        else cout << "Store name already in use, please choose another one\n";
    }

    cout << "your address : ";
    getline(cin >> ws, alamat);

    if (hasLoggedIn()) {
        buyers[current_buyer_index].bikinSellerAcc (store,alamat);
    } else if (!buyers.empty()) {
        buyers.back().bikinSellerAcc(store, alamat);
    } else {
        cout << "Error: no buyer found to attach the shop.\n";
        return;
    }

    saveAllDataCSV();
    cout << "Congratulations, your shop account has been successfully created.\n";
    
}

void display_all_your_purchased_items(){
    cout << "============================================================\n";
    cout << "                     PURCHASED ITEMS                        \n";
    cout << "============================================================\n";

    double total_spent = 0.0;
    cout << left << setw(5) << "No"
        << setw(25) << "Product Name"
        << setw(10) << "Qty"
        << setw(15) << "Price (Rp)"
        << setw(15) << "Total" << "\n";
    cout << "____________________________________________________________\n";

    for (int i = 0; i < allOrders.size(); ++i) {
        if(allOrders[i].getbuyNAme() == buyers[current_buyer_index].getName()){
            cout << left << setw(5) << i + 1
            << setw(25) << allOrders[i].getName()
            << setw(10) << allOrders[i].getJumlah()
            << setw(15) << fixed << setprecision(2) << allOrders[i].getHarga()
            << setw(15) << allOrders[i].getamount() << "\n";
            total_spent += allOrders[i].getamount();
        }
    }

    if (total_spent <1) {
        cout << "You haven't purchased any items yet.\n";
        cout << "========================================================\n";
        return;
    }

    cout << "-------------------------------------------------------------\n";
    cout << "Total Spent: Rp. " << fixed << setprecision(2) << total_spent << "\n";
    cout << "______________________________________________________________\n";
}

void register_menu(bool toko) {
    string name,password,pin;
    cout << "\n==============================================\n";
    cout << "           CREATE NEW USER ACCOUNT            \n";
    cout << "==============================================\n\n";

    cout << endl << "";

    while (true){
        cout << "Full name    : ";
        getline(cin >> ws, name);

        if(cek_duplicate_name(name)) break;
        else cout << "name already in use, please choose another one\n";
    }
    

    cout << endl << "Password     : ";
    getline(cin >> ws, password);

    buyers.push_back(buyerAcc(name,password,toko));

    cout << "\nAccount created successfully for \"" << name << "\"!\n\n";

    cout << "----------------------------------------------\n";
    cout << "create your BANK ACCOUNT (PIN + Balance)\n";
    cout << "----------------------------------------------\n\n";

    cout << "Bank PIN       : ";
    getline(cin >> ws, pin);
    double saldo = regSaldo();

    buyers.back().bikinBankAcc(pin, saldo);
    saveAllDataCSV();

    cout << "\n==============================================\n";
    cout << "          REGISTRATION COMPLETE               \n";
    cout << "==============================================\n\n";
    cout << "Account holder : " << buyers.back().getName() << "\n";
    cout << "Bank PIN       : " << buyers.back().getPin() << " \n";
    cout << "Balance        : RP." << buyers.back().getSaldo() << "\n\n";
    cout << "Thank you! Your bank account has been created.\n";
    
    if(toko) {   
        string store, alamat;
        cout << "----------------------------------------------\n";
        cout << "now create your shop account\n";
        cout << "----------------------------------------------\n\n";
        while (true){
            cout << "Store Name   : ";
            getline(cin >> ws, store);

            if(cek_duplicate_store_name(store)) break;
            else cout << "Store name already in use, please choose another one\n";
        }
        cout << "your address : ";
        getline(cin >> ws, alamat);
        buyers.back().bikinSellerAcc(store, alamat);
        saveAllDataCSV();
        cout << "Congratulations, your shop account has been successfully created.\n";
    }
    
    cout << "Please log back in with your account!\n\n";
}

void bank_fitur_menu () {
    if (!hasLoggedIn()) {
        cout << "Error: no account logged in.\n";
        return;
    }

    string pin,saldo;
    double balance;
    bank_fitur_prompt bank_fitur = BANK_FITUR_MENU;
    int pilihan = 0;

    while (bank_fitur != BALIK1) {  
        switch (bank_fitur){
        
        case BANK_FITUR_MENU :
            cout << "==============================================\n";
            cout << "              BANKING FEATURES MENU           \n";
            cout << "==============================================\n\n";
            cout << "1. Check Balance\n";
            cout << "2. Top Up\n";
            cout << "3. Withdraw\n";
            cout << "4. List Cash Flow (Credit/Debit)\n";
            cout << "5. Back to Buyer Menu\n";
            cout << "----------------------------------------------\n";
            cout << "Select an option: ";
            cin >> pilihan;

            if(pilihan == 6) pilihan = 7;
            bank_fitur = static_cast <bank_fitur_prompt> (pilihan-1);

            break;

        case CEK_SALDO:{
            string mundur;
            buyers[current_buyer_index].tampilinSaldo();
            cout << "To return to the bank menu, provide any input : ";
            cin.ignore();
            getline(cin >> ws, mundur);
            bank_fitur = BANK_FITUR_MENU;
            break;
        }
        case TOP_UP:
            while (true){
                cout << "\n----------------------------------------------\n";
                cout << "                   TOP UP                     \n";
                cout << "----------------------------------------------\n\n";

                cout << "Bank PIN       : ";
                
                getline(cin >> ws, pin);
                if (buyers[current_buyer_index].getPin() != pin){
                    cout << "\n Invalid PIN! \n";
                }else{
                    break;
                }
            }
            balance = regSaldo();
            buyers[current_buyer_index].addsaldo(balance);
            buyers[current_buyer_index].tambahTransaksi(balance,true);
            saveAllDataCSV();
            cout << "\n Your balance has been added successfully! \n";

            bank_fitur = BANK_FITUR_MENU;
            break;

        case WITHDRAW:
            while (true){
                cout << "\n----------------------------------------------\n";
                cout << "                 WITHDRAWAL                   \n";
                cout << "----------------------------------------------\n\n";

                cout << "Bank PIN       : ";
                cin.clear();
                getline(cin >> ws, pin);
                if (buyers[current_buyer_index].getPin() != pin){
                    cout << "\n Invalid PIN! \n";
                }else{
                    break;
                }
            }
            balance = regSaldo();
            while (!buyers[current_buyer_index].withdraw(balance)){
                cout << "your balance is insufficient \n";
                balance = regSaldo();
            }
            
            cout << "\n Your balance withdrawal was successful! \n";
            buyers[current_buyer_index].tambahTransaksi(balance,false);
            saveAllDataCSV();
            bank_fitur = BANK_FITUR_MENU;
            break;

        case CASH_FLOW : {
            string mundur;
            buyers[current_buyer_index].tampilinTransaksi();
            cout << "To return to the bank menu, provide any input : ";
            cin.ignore();
            getline(cin >> ws, mundur);
            bank_fitur = BANK_FITUR_MENU;

            break;
        }
        case BALIK1 :
            cout << "Exiting. " <<endl;
            break;

        default:
            cout << "Invalid option." << endl;
            bank_fitur = BANK_FITUR_MENU;
            cin.clear();
            cin.ignore();
            break;
        }
    }
}

void add_income (double balance, string sellerStoreName) {
    for (int i = 0; i < buyers.size(); i++){
        if(buyers[i].getStoreName() == sellerStoreName) {
            buyers[i].addsaldo(balance);
            buyers[i].tambahTransaksi(balance,true);
            saveAllDataCSV();
        }
    }
}

void display_available_product () {
    int no = 1;
    cout << "\n======================================================\n";
    cout << "                 LIST AVAILABLE PRODUCT                 \n";
    cout << "========================================================\n";
    cout << left << setw(5)  << "No."
        << setw(20) << "Nama Produk"
        << setw(10) << "Harga"
        << setw(10) << "Stok"
        << setw(10) << "Store Name"<< endl;
    cout <<"--------------------------------------------------------\n";

    for (const auto& produk : allProduct ) {
        cout << left << setw(7)  << no
                << setw(22) << produk.getName()
                << "Rp " << setw(8) << fixed << setprecision(0) << produk.getPrice()
                << setw(7)  << produk.getJumlah()
                << setw(12) << produk.getSellerName() << endl;
        
        
        ++no;
    }
    if(allProduct.size() < 1)  cout << left << setw(5) << no << setw(20) << "no products available" << setw(10) << "" << setw(10) << ""  << endl;;
    cout << "=======================================================\n";
}

void display_income_seller () {
    cout << "======================================================================\n";
    cout << "                            YOUR INCOME                              \n";
    cout << "======================================================================\n";

    double total_spent = 0.0;
    cout << left << setw(5) << "No"
        << setw(25) << "Product Name"
        << setw(10) << "Qty"
        << setw(15) << "Price (Rp)"
        << setw(15) << "Total" 
        << setw(15) << "Buyer Name" << "\n";
    cout << "_____________________________________________________________________\n";

    for (int i = 0; i < allOrders.size(); ++i) {
        if(allOrders[i].getSellerName() == buyers[current_buyer_index].getStoreName()){
            cout << left << setw(5) << i + 1
            << setw(25) << allOrders[i].getName()
            << setw(10) << allOrders[i].getJumlah()
            << setw(15) << fixed << setprecision(2) << allOrders[i].getHarga()
            << setw(15) << allOrders[i].getamount() 
            << setw(15) << allOrders[i].getbuyNAme() << "\n";
            total_spent += allOrders[i].getamount();
        }
    }

    if (total_spent <1) {
        cout << "You haven't purchased any items yet.\n";
        cout << "=================================================================\n";
        return;
    }

    cout << "---------------------------------------------------------------------\n";
    cout << "Total income: Rp. " << fixed << setprecision(2) << total_spent << "\n";
    cout << "_____________________________________________________________________\n";
}

bool addprodct (string skuId, string nama, string sellerName1 ,int jumlah1, double price1) {
    for ( int i = 0; i < allProduct.size();i++){
        if(allProduct[i].getSku () == skuId) return false;
    }

    allProduct.push_back(product(skuId,nama,sellerName1,jumlah1,price1));
    return true;
}

void add_product_menu(){
    string skuID;
    string nameProduct;
    int jumlah;
    double price;

    while (true){
        cout << "======================================\n";
        cout << "           ADD NEW PRODUCT            \n";
        cout << "======================================\n";
        
        cout << "Product Name        : ";
        getline(cin>>ws, nameProduct);
        price = regSaldo();
        while (true) {
            cout << "Product Stock       : ";
            cin >> jumlah;

            if (cin.fail()) { 
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Must be a number.\n";
            } else break;
            
        }
        cin.ignore();
        cout << "Product SKU         : ";
        getline(cin>>ws, skuID);

        if(addprodct(skuID,nameProduct,buyers[current_buyer_index].getStoreName(),jumlah,price)) break;
    }
    

    cout << "\n-------------------------------------------------------------\n";
    cout<<"Product added successfully.\n";
    buyers[current_buyer_index].addproduct(skuID);
    saveAllDataCSV();
}

void Procesing_orders (int indeks, int jumlah) {
    string pin;
    double saldo = buyers[current_buyer_index].getSaldo();
    double total = jumlah * allProduct[indeks].getPrice();

    cout << "\n------------------------------------------------.\n";
    cout << "                     PAYMENT                     \n";
    cout << "-------------------------------------------------\n";

    while (true){
        cout << "Insert Bank PIN       : ";
        cin.clear();
        getline(cin >> ws, pin);
        if (buyers[current_buyer_index].getPin() != pin){
            cout << "\n Invalid PIN! \n";
        }else{
            break;
        }
    }

    if(saldo >= total) {
        buyers[current_buyer_index].withdraw(total);
        buyers[current_buyer_index].tambahTransaksi(total,false);
        allProduct[indeks].decProduct(jumlah);
        allOrders.push_back(order(allProduct[indeks],buyers[current_buyer_index].getName(),total,jumlah));
        add_income(total, allOrders.back().getSellerName());
        saveAllDataCSV();
        cout << "\n Your Payment was successful! \n";
    }else {
        cout << "Payment failed \nYour balance is insufficient\n";
    }
    
}

void buy_product () {
    int pilih,jumlah;
    string conf;

    display_available_product();
    while (true) {
        cout << "select the product you want to buy: ";
        cin >> pilih;

        if (cin.fail() || pilih > allProduct.size()) { 
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!\n";
        } else break;
        
    }

    pilih--;

    cout << "\n==============================================\n";
    cout << "                 BUY PRODUCT                  \n";
    cout << "==============================================\n\n";
    cout << "Product Name : " << allProduct[pilih].getName() << "\n";
    cout << "Price        : RP." << allProduct[pilih].getPrice() << " \n";
    cout << "Stock        : " << allProduct[pilih].getJumlah() << "\n\n";
    cout << "________________________________________________.\n";
    while (true) {
        cout << "Enter the number of products you want to buy: ";
        cin >> jumlah;

        if (cin.fail() || jumlah >  allProduct[pilih].getJumlah()) { 
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!\n";
        } else break;
        
    }

    cout << "Are you sure you want to buy this product (Y/N)?: ";
    getline(cin >> ws, conf);
    if(conf == "y" || conf == "Y") {Procesing_orders(pilih,jumlah);}
}

void toko_menu(){
    menu_toko promt = TOKO_MENU;
    while(promt!=EXIT_MENU4){
        
        switch (promt)
        {
            
            case TOKO_MENU:{
                cout << "=============================================================\n";
                cout << "                       Store Menu                    \n";
                cout << "=============================================================\n";
                cout << left;
                cout << " 1. List Produk\n";
                cout << " 2. Upload Produk\n";
                cout << " 3. Edit Produk\n";
                cout << " 4. View Income\n";
                cout << " 5. Exit to Seller Menu\n";
                cout << "-------------------------------------------------------------\n";
                cout << " Select an option: ";
                int choice;
                cin >> choice;
                promt = static_cast<menu_toko>(choice-1);
                break;
            }
            case LIST_PRODUK:{
                buyers[current_buyer_index].display_products();
                string mundur;
                cout << "To return to the Store menu, provide any input : ";
                getline(cin >> ws, mundur);
                promt = TOKO_MENU;
                break;
            }
            case UPLOAD_PRODUK:{
                add_product_menu();
                promt = TOKO_MENU;
                break;
            }
            case EDIT_PRODUK:{
                int stok,banyakProduk,pilih;
                double harga;

                buyers[current_buyer_index].display_products();
                banyakProduk = buyers[current_buyer_index].getJumlahProduk();

                while (true) {
                    cout << "select the product you want to edit: ";
                    cin >> pilih;

                    if (cin.fail() || pilih > banyakProduk ) { 
                        cin.clear(); 
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input!\n";
                    } else break;
                    
                }

                
                while (true) {
                    cout << "Insert new stock    : ";
                    cin >> stok;

                    if (cin.fail()) { 
                        cin.clear(); 
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input! Must be a number.\n";
                    } else break;
                    
                }
                harga = regSaldo();

                buyers[current_buyer_index].edit_produk(pilih,stok,harga);
                saveAllDataCSV();
                promt = TOKO_MENU;
                break;
            }
            case PESANAN:{
                string mundur;
                display_income_seller();
                cout << "To return to the store menu, provide any input : ";
                cin.ignore();
                getline(cin >> ws, mundur);
                promt = TOKO_MENU;
                break;
            }case EXIT_MENU4:
                cout<<"back to seller menu\n";
                promt = EXIT_MENU4;
                break;
            default:
                cout<<"Invalid choice.\n";
                cin.clear();
                cin.ignore();
                promt = TOKO_MENU;
                break;
            }

    }

}

void seller_menu () {
    seller_menu_prompt prompt_seller = BUYER_MENU1;
    int pilihan = 0;

    while (prompt_seller != BALIK2) {  
        switch (prompt_seller){
        
        case BUYER_MENU1 :
            cout << "==============================================\n";
            cout << "                    Main MENU                \n";
            cout << "==============================================\n\n";
            cout << "1. Purchase Item (limited by balance)\n";
            cout << "2. List All Orders\n";
            cout << "3. Banking Related Features\n";
            cout << "4. Seller Related Features\n";
            cout << "5. Log Out\n";
            cout << "----------------------------------------------\n";
            cout << "Select an option: ";
            cin >> pilihan;

            if(pilihan == 6) pilihan = 7;
            prompt_seller = static_cast <seller_menu_prompt> (pilihan-1);
            cout<<prompt_seller;
            break;
        case PRODUCT_LIST1:
            buy_product();
            prompt_seller = BUYER_MENU1;
            break;

        case ORDER_LIST1: {
            string mundur;
            display_all_your_purchased_items();
            cout << "To return to the Main menu, provide any input : ";
            cin.ignore();
            getline(cin >> ws, mundur);
            prompt_seller = BUYER_MENU1;
            break;
        }
        case TO_BANK1 :
            bank_fitur_menu();
            prompt_seller = BUYER_MENU1;
            break;
        
        case SELLER_MENU :
            toko_menu();
            prompt_seller = BUYER_MENU1;
            break;

        case BALIK2 :
            cout << "Exiting. " <<endl;
            prompt_seller = BALIK2;
            break;

        default:
            cout << "Invalid option." << endl;
            prompt_seller = BUYER_MENU1;
            cin.clear();
            cin.ignore();
            break;
        }
    }
}

void buyer_menu () {
    buyer_menu_prompt prompt_buyer = BUYER_MENU;
    int pilihan = 0;

    while (prompt_buyer != BALIK) {  
        switch (prompt_buyer){
        
        case BUYER_MENU :
            cout << "==============================================\n";
            cout << "                    BUYER MENU                \n";
            cout << "==============================================\n\n";
            cout << "1. Purchase Item (limited by balance)\n";
            cout << "2. List All Orders\n";
            cout << "3. Banking Related Features\n";
            cout << "4. Upgrade to seller account\n";
            cout << "5. Log Out\n";
            cout << "----------------------------------------------\n";
            cout << "Select an option: ";
            cin >> pilihan;

            if(pilihan == 6) pilihan = 7;
            prompt_buyer = static_cast <buyer_menu_prompt> (pilihan-1);

            break;
        case PRODUCT_LIST:{
            buy_product();
            prompt_buyer = BUYER_MENU;
            break;
        }
        case ORDER_LIST:{
            string mundur;
            display_all_your_purchased_items();
            cout << "To return to the main menu, provide any input : ";
            cin.ignore();
            getline(cin >> ws, mundur);
            prompt_buyer = BUYER_MENU;
            break;
        }    
        case TO_BANK :
            bank_fitur_menu();
            prompt_buyer = BUYER_MENU;
            break;

        case UPSELLER :
            buatToko();
            seller_menu();
            prompt_buyer = BALIK;
            break;

        case BALIK :
            current_buyer_index = -1;
            cout << "Exiting. " <<endl;
            prompt_buyer = BALIK;
            break;

        default:
            cout << "Invalid option." << endl;
            prompt_buyer = BUYER_MENU;
            cin.clear();
            cin.ignore();
            break;
        }
    }
}

void register_menu_start() {
    RegisterMenu reg_menu = MENU1;
    int choice2;

    bool running = true;
    while (running) {
        switch (reg_menu) {
            case MENU1:
                cout << "=============================================================\n";
                cout << "                        REGISTER Menu                        \n";
                cout << "=============================================================\n";
                cout << "1. Register as Buyer" << endl;
                cout << "2. Register as Seller" << endl;
                cout << "3. Back to Main Menu" << endl;
                cout << "Choice: ";
                cin >> choice2;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input. Try again.\n";
                    break;
                }

                if(choice2 == 4) choice2 = 5;
                reg_menu = static_cast <RegisterMenu> (choice2-1);
                break;

            case AS_BUYER:
                register_menu(false); 
                reg_menu = EXIT3;
                break;

            case AS_SELLER:
                register_menu(true); 
                reg_menu = EXIT3;
                break;

            case EXIT3:
                running = false; 
                break;
            default :
                cout << "Invalid option." << endl;
                reg_menu = MENU1;
                cin.clear();
                cin.ignore();
                break;
        }
    }
}

void login_menu () {
    loginMenu log_menu = MENU2;
    int choice2;
    string name, password;

    bool running = true;
    while (running) {
        switch (log_menu) {
            case MENU2:
                cout << "=============================================================\n";
                cout << "                           Login Menu                        \n";
                cout << "=============================================================\n";
                cout << "1. login as Buyer" << endl;
                cout << "2. login as Seller" << endl;
                cout << "3. Back to main menu" << endl;
                cin >> choice2;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input. Try again.\n";
                    break;
                }

                if(choice2 == 4) choice2 = 5;
                log_menu = static_cast <loginMenu> (choice2-1);
                break;

            case LOG_BUYER:
                while(true){
                    cout << "\n----------------------------------------------\n";
                    cout << "Please enter name and pasword for Login. \n";
                    cout << "----------------------------------------------\n";
                    cout << "Name     : ";
                    getline(cin >> ws, name);

                    cout << "Password : ";
                    getline(cin >> ws, password);

                    bool login = Login_verif(name, password);
                    
                    if(login){
                        if(hasLoggedIn() && buyers[current_buyer_index].is_seller()){
                            cout<<"You are also a seller, redirecting to seller menu.\n";
                            seller_menu();
                        }else{  
                            cout<<"You are logged in as buyer, redirecting to buyer menu.\n";
                            cout<<"Login-successful!\n";
                            buyer_menu();
                        }
                        log_menu =  MENU2;
                        break;
                    }else{
                        cout<<"Login failed, try again.\n";
                    }
                } 
                log_menu = EXIT4;
                break;

            case LOG_SELLER:
                while(true){
                    cout << "\n----------------------------------------------\n";
                    cout << "Please enter name and pasword for Login." << name << "\n";
                    cout << "----------------------------------------------\n";
                    cout << "Name     : ";
                    getline(cin >> ws, name);

                    cout << "Password : ";
                    getline(cin >> ws, password);

                    bool login = Login_verif(name, password);
                    
                    if(login){
                        if(hasLoggedIn() && buyers[current_buyer_index].is_seller()){
                            seller_menu();
                        }else{  
                            cout<<"You are not a seller, please login as buyer or register as seller.\n";
                        }
                        log_menu = EXIT4;
                        break;
                    }else{
                        cout<<"Login failed, try again.\n";
                    }
                } 
                log_menu = EXIT4;
                break;

            case EXIT4:
                running = false; 
                break;
            default :
                cout << "Invalid option." << endl;
                log_menu = MENU2;
                cin.clear();
                cin.ignore();
                break;
        }
    }
}

int main () {
    if (!loadAllDataCSV()) {
        cout << "Warning: failed to load data CSV (files may not exist yet).\n";
    }

    main_prompt prompt_utama = MAIN_MENU;
    int pilihan = 0;

    while (prompt_utama != EXIT) {  
        switch (prompt_utama){
        
        case MAIN_MENU :
            cout << "\n==============================================\n";
            cout << "              Select an option                \n";
            cout << "==============================================\n\n";
            cout << "1. Login" << endl;
            cout << "2. Register" << endl;
            cout << "3. Exit" << endl;
            cout << "select your option : ";cin >>pilihan;

            if(pilihan == 4) pilihan = 5;
            prompt_utama = static_cast <main_prompt> (pilihan-1);

            break;
        case LOGIN:
            login_menu();
            prompt_utama = MAIN_MENU;
            break;
        case REGISTER:
            register_menu_start();
            
            prompt_utama = LOGIN;
            break;
        case EXIT :
            cout << "Exiting. " <<endl;
            prompt_utama = EXIT;
            break;
        default:
            cout << "Invalid option." << endl;
            prompt_utama = MAIN_MENU;
            cin.clear();
            cin.ignore();
            break;
        }
    }
    if (!saveAllDataCSV()) {
        cout << "Warning: failed to save CSV files.\n";
    } else {
        cout << "Data saved to CSV.\n";
    }

    return 0;
}
