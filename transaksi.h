// transaksi.h
#ifndef TRANSAKSI_H
#define TRANSAKSI_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

static string currentDateString() {
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

class transaksi{
private:
    double amount;
    string date;
public:
    transaksi(double amount1, const string& date1 = "") 
        : amount(amount1), date(date1.empty() ? currentDateString() : date1) {};

    double getAmount () const { return amount; }
    string getDate () const { return date; }
};

#endif
