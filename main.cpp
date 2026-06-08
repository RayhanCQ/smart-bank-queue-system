#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>

using namespace std;

class Nasabah
{
public:
    int nomorRekening;
    string nama;
    double saldo;

    Nasabah(int rek, string n, double s)
        : nomorRekening(rek), nama(n), saldo(s) {}
};

class Transaksi
{
public:
    int nomorRekening;
    string nama;
    string jenis;
    double nominal;

    Transaksi(int rek, string n, string j, double nom)
        : nomorRekening(rek), nama(n), jenis(j), nominal(nom) {}
};

queue<Nasabah> antrianTeller;
stack<Transaksi> riwayatTransaksi;
vector<Nasabah> daftarNasabah;

mutex mtx;

// =======================
// Binary Search
// =======================
int binarySearchRekening(int target)
{
    int left = 0;
    int right = daftarNasabah.size() - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (daftarNasabah[mid].nomorRekening == target)
            return mid;

        if (daftarNasabah[mid].nomorRekening < target)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return -1;
}

// =======================
// Queue
// =======================
void ambilAntrian()
{
    int rekening;

    cout << "\nMasukkan Nomor Rekening: ";
    cin >> rekening;

    int idx = binarySearchRekening(rekening);

    if (idx == -1)
    {
        cout << "Nasabah tidak ditemukan!\n";
        return;
    }

    antrianTeller.push(daftarNasabah[idx]);

    cout << daftarNasabah[idx].nama
         << " berhasil masuk antrian.\n";
}

void panggilNasabah()
{
    if (antrianTeller.empty())
    {
        cout << "\nAntrian kosong.\n";
        return;
    }

    Nasabah n = antrianTeller.front();
    antrianTeller.pop();

    cout << "\n================================\n";
    cout << "Sedang Melayani:\n";
    cout << "Nama      : " << n.nama << endl;
    cout << "Rekening  : " << n.nomorRekening << endl;
    cout << "Saldo     : Rp" << n.saldo << endl;
    cout << "================================\n";
}

// =======================
// Search
// =======================
void cariRekening()
{
    int rekening;

    cout << "\nMasukkan Nomor Rekening: ";
    cin >> rekening;

    int idx = binarySearchRekening(rekening);

    if (idx == -1)
    {
        cout << "Data tidak ditemukan.\n";
        return;
    }

    cout << "\nData Ditemukan\n";
    cout << "Nama      : "
         << daftarNasabah[idx].nama << endl;

    cout << "Rekening  : "
         << daftarNasabah[idx].nomorRekening << endl;

    cout << "Saldo     : Rp"
         << daftarNasabah[idx].saldo << endl;
}

// =======================
// Stack
// =======================
void prosesTransaksi()
{
    int rekening;

    cout << "\nMasukkan Nomor Rekening: ";
    cin >> rekening;

    int idx = binarySearchRekening(rekening);

    if (idx == -1)
    {
        cout << "Nasabah tidak ditemukan.\n";
        return;
    }

    int pilihan;
    double nominal;

    cout << "\n1. Setor Tunai\n";
    cout << "2. Tarik Tunai\n";
    cout << "Pilih: ";
    cin >> pilihan;

    cout << "Nominal: ";
    cin >> nominal;

    string jenis;

    if (pilihan == 1)
    {
        daftarNasabah[idx].saldo += nominal;
        jenis = "Setor";
    }
    else
    {
        if (nominal > daftarNasabah[idx].saldo)
        {
            cout << "Saldo tidak cukup!\n";
            return;
        }

        daftarNasabah[idx].saldo -= nominal;
        jenis = "Tarik";
    }

    riwayatTransaksi.push(
        Transaksi(
            daftarNasabah[idx].nomorRekening,
            daftarNasabah[idx].nama,
            jenis,
            nominal));

    cout << "\nTransaksi berhasil.\n";
    cout << "Saldo sekarang : Rp"
         << daftarNasabah[idx].saldo << endl;
}

void tampilRiwayat()
{

    if (riwayatTransaksi.empty())
    {
        cout << "\nBelum ada transaksi.\n";
        return;
    }

    stack<Transaksi> temp = riwayatTransaksi;

    cout << "\n===== RIWAYAT TRANSAKSI =====\n";

    while (!temp.empty())
    {

        Transaksi t = temp.top();

        cout << "\nNama      : " << t.nama;
        cout << "\nRekening  : " << t.nomorRekening;
        cout << "\nJenis     : " << t.jenis;
        cout << "\nNominal   : Rp" << t.nominal;
        cout << "\n-------------------------";

        temp.pop();
    }

    cout << endl;
}

// =======================
// Parallel Programming
// =======================
void prosesCabang(string namaCabang,
                  int jumlahTransaksi)
{

    mtx.lock();

    cout << "\nCabang "
         << namaCabang
         << " memproses "
         << jumlahTransaksi
         << " transaksi.";

    mtx.unlock();
}

void rekapCabang()
{

    cout << "\nMenjalankan Rekap Harian...\n";

    thread t1(prosesCabang,
              "Jakarta", 150);

    thread t2(prosesCabang,
              "Bandung", 120);

    thread t3(prosesCabang,
              "Surabaya", 170);

    thread t4(prosesCabang,
              "Semarang", 100);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    cout << "\n\nRekap selesai.\n";
}

// =======================
// Big O
// =======================
void analisisBigO()
{

    cout << "\n===== ANALISIS BIG O =====\n";

    cout << "Queue Enqueue      : O(1)\n";
    cout << "Queue Dequeue      : O(1)\n";
    cout << "Stack Push         : O(1)\n";
    cout << "Stack Pop          : O(1)\n";
    cout << "Binary Search      : O(log n)\n";
    cout << "Parallel Rekap     : O(n/thread)\n";
}

// =======================
// Data Awal
// =======================
void initData()
{

    daftarNasabah.push_back(
        Nasabah(1001, "Budi", 5000000));

    daftarNasabah.push_back(
        Nasabah(1002, "Siti", 3000000));

    daftarNasabah.push_back(
        Nasabah(1003, "Andi", 4500000));

    daftarNasabah.push_back(
        Nasabah(1004, "Rina", 2500000));

    daftarNasabah.push_back(
        Nasabah(1005, "Doni", 7000000));

    sort(
        daftarNasabah.begin(),
        daftarNasabah.end(),
        [](Nasabah a, Nasabah b)
        {
            return a.nomorRekening <
                   b.nomorRekening;
        });
}

// =======================
// Main
// =======================
int main()
{

    initData();

    int pilihan;

    do
    {

        cout << "\n";
        cout << "====================================\n";
        cout << " SMART BANK QUEUE SYSTEM\n";
        cout << "====================================\n";
        cout << "1. Ambil Nomor Antrian\n";
        cout << "2. Panggil Nasabah Berikutnya\n";
        cout << "3. Cari Rekening Nasabah\n";
        cout << "4. Proses Transaksi\n";
        cout << "5. Lihat Riwayat Transaksi\n";
        cout << "6. Rekap Cabang\n";
        cout << "7. Analisis Big O\n";
        cout << "0. Keluar\n";
        cout << "====================================\n";
        cout << "Pilih Menu : ";
        cin >> pilihan;

        switch (pilihan)
        {

        case 1:
            ambilAntrian();
            break;

        case 2:
            panggilNasabah();
            break;

        case 3:
            cariRekening();
            break;

        case 4:
            prosesTransaksi();
            break;

        case 5:
            tampilRiwayat();
            break;

        case 6:
            rekapCabang();
            break;

        case 7:
            analisisBigO();
            break;

        case 0:
            cout << "\nProgram selesai.\n";
            break;

        default:
            cout << "\nMenu tidak valid.\n";
        }

    } while (pilihan != 0);

    return 0;
}