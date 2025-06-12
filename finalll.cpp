#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <limits>
#include <iomanip>
#include <string>
#include <windows.h>
#include <random>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

using namespace std;

struct Mahasiswa {
    string nama;
    string nim;
    string jurusan;
    float ipk;
};

vector<Mahasiswa> daftarMahasiswa;
map<int, vector<Mahasiswa>> pembagianKelompok;
vector<map<int, vector<Mahasiswa>>> riwayatPembagian;

// Membersihkan buffer input untuk mencegah kesalahan input
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Menunggu tombol Enter ditekan dan membersihkan layar
void waitForEnterAndClear() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.get();
    clearInputBuffer();
    system(CLEAR);
}

// Menampilkan layar loading dengan animasi saat program dimulai
void loadingScreen() {
    system("color 0A");
    system(CLEAR);
    cout << "=====================================" << endl;
    cout << "|           WELCOME TO APP          |" << endl;
    cout << "|          TEAM BUILDER PRO         |" << endl;
    cout << "=====================================\n";
    cout << "\nMemuat data";

    for (int i = 0; i < 5; i++) {
        cout << ".";
        Sleep(500);
    }
    cout << "\nSukses! Menyiapkan menu utama...\n";
    Sleep(1000);
    system(CLEAR);
}

// Memvalidasi input numerik dan mengembalikan true jika berhasil
bool inputNumerik(int &value, const string &prompt) {
    cout << prompt;
    if (cin >> value) return true;
    cout << "Input harus angka.\n";
    clearInputBuffer();
    return false;
}

// Memvalidasi input string agar tidak kosong dan mengembalikannya
string inputString(const string &prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!input.empty()) return input;
        cout << "Input tidak boleh kosong.\n";
    }
}

// Memvalidasi NIM (harus 10 digit angka)
bool validasiNIM(const string &nim) {
    return nim.length() == 10 && all_of(nim.begin(), nim.end(), ::isdigit);
}

// Memeriksa apakah NIM sudah ada di daftar mahasiswa
bool cekDuplikasiNIM(const string &nim) {
    for (const auto &m : daftarMahasiswa) {
        if (m.nim == nim) return true;
    }
    return false;
}

// Menambahkan data mahasiswa baru ke daftar
void tambahMahasiswa() {
    system(CLEAR);
    cout << "===== Tambah Mahasiswa =====\n\n";
    Mahasiswa m;
    clearInputBuffer();
    m.nama = inputString("Nama: ");

    while (true) {
        m.nim = inputString("NIM: ");
        if (!validasiNIM(m.nim)) {
            cout << "NIM harus 10 digit angka.\n";
            continue;
        }
        if (cekDuplikasiNIM(m.nim)) {
            cout << "NIM sudah terdaftar. Gunakan NIM lain.\n";
            continue;
        }
        break;
    }

    m.jurusan = inputString("Jurusan: ");

    while (true) {
        cout << "IPK: ";
        if (cin >> m.ipk && m.ipk >= 0.0 && m.ipk <= 4.0) break;
        cout << "IPK tidak valid (0.0 - 4.0).\n";
        clearInputBuffer();
    }

    daftarMahasiswa.push_back(m);
    cout << "\033[1;32mMahasiswa berhasil ditambahkan.\033[0m\n";
}

// Menampilkan semua data mahasiswa dalam format tabel
void tampilkanSemua() {
    system(CLEAR);
    cout << "===== Daftar Semua Mahasiswa =====\n\n";
    if (daftarMahasiswa.empty()) {
        cout << "Belum ada data mahasiswa.\n";
        return;
    }
    cout << left << setw(5) << "No" << setw(20) << "Nama" << setw(15) << "NIM"
         << setw(20) << "Jurusan" << setw(5) << "IPK" << endl;
    cout << string(65, '-') << endl;
    for (size_t i = 0; i < daftarMahasiswa.size(); ++i) {
        cout << left << setw(5) << i + 1 << setw(20) << daftarMahasiswa[i].nama
             << setw(15) << daftarMahasiswa[i].nim << setw(20) << daftarMahasiswa[i].jurusan
             << setw(5) << fixed << setprecision(2) << daftarMahasiswa[i].ipk << endl;
    }
}

// Menampilkan pembagian mahasiswa ke dalam pot berdasarkan IPK
void tampilkanPot() {
    system(CLEAR);
    cout << "===== Pot Berdasarkan IPK =====\n\n";
    if (daftarMahasiswa.empty()) {
        cout << "Belum ada data mahasiswa.\n";
        return;
    }
    vector<Mahasiswa> sorted = daftarMahasiswa;
    sort(sorted.begin(), sorted.end(), [](const Mahasiswa& a, const Mahasiswa& b) {
        return a.ipk > b.ipk;
    });
    int jumlahPot = 3;
    int perPot = sorted.size() / jumlahPot + (sorted.size() % jumlahPot != 0);

    for (int i = 0; i < jumlahPot; ++i) {
        cout << "\nPot " << i + 1 << ":\n";
        cout << left << setw(5) << "No" << setw(20) << "Nama" << setw(15) << "NIM"
             << setw(20) << "Jurusan" << setw(5) << "IPK" << endl;
        cout << string(65, '-') << endl;
        int start = i * perPot;
        int end = min((i + 1) * perPot, (int)sorted.size());
        for (int j = start; j < end; ++j) {
            cout << left << setw(5) << j - start + 1 << setw(20) << sorted[j].nama
                 << setw(15) << sorted[j].nim << setw(20) << sorted[j].jurusan
                 << setw(5) << fixed << setprecision(2) << sorted[j].ipk << endl;
        }
    }
}

// Mengubah data mahasiswa berdasarkan nomor urut
void ubahMahasiswa() {
    system(CLEAR);
    cout << "===== Ubah Data Mahasiswa =====\n\n";
    tampilkanSemua();
    if (daftarMahasiswa.empty()) return;
    int idx;
    while (!inputNumerik(idx, "Pilih nomor mahasiswa yang ingin diubah: ") ||
           idx < 1 || idx > daftarMahasiswa.size()) {
        cout << "Nomor tidak valid.\n";
    }
    Mahasiswa& m = daftarMahasiswa[idx - 1];
    clearInputBuffer();
    m.nama = inputString("Nama baru: ");

    while (true) {
        m.nim = inputString("NIM baru: ");
        if (!validasiNIM(m.nim)) {
            cout << "NIM harus 10 digit angka.\n";
            continue;
        }
        if (cekDuplikasiNIM(m.nim) && m.nim != daftarMahasiswa[idx - 1].nim) {
            cout << "NIM sudah terdaftar. Gunakan NIM lain.\n";
            continue;
        }
        break;
    }

    m.jurusan = inputString("Jurusan baru: ");

    while (true) {
        cout << "IPK baru: ";
        if (cin >> m.ipk && m.ipk >= 0.0 && m.ipk <= 4.0) break;
        cout << "IPK tidak valid (0.0 - 4.0).\n";
        clearInputBuffer();
    }

    cout << "\033[1;32mData berhasil diubah.\033[0m\n";
}

// Menghapus data mahasiswa berdasarkan nomor urut
void hapusMahasiswa() {
    system(CLEAR);
    cout << "===== Hapus Mahasiswa =====\n\n";
    tampilkanSemua();
    if (daftarMahasiswa.empty()) return;
    int idx;
    while (!inputNumerik(idx, "Pilih nomor mahasiswa yang ingin dihapus: ") ||
           idx < 1 || idx > daftarMahasiswa.size()) {
        cout << "Nomor tidak valid.\n";
    }
    daftarMahasiswa.erase(daftarMahasiswa.begin() + (idx - 1));
    cout << "\033[1;32mData berhasil dihapus.\033[0m\n";
}

// Membagi mahasiswa ke kelompok secara acak dengan distribusi IPK yang seimbang
void bagiKelompokAcak() {
    system(CLEAR);
    cout << "===== Bagi Kelompok Secara Acak =====\n\n";
    int jumlahKelompok;
    while (!inputNumerik(jumlahKelompok, "Masukkan jumlah kelompok: ") ||
           jumlahKelompok <= 0 || jumlahKelompok > daftarMahasiswa.size()) {
        cout << "Jumlah kelompok tidak valid atau melebihi jumlah mahasiswa.\n";
    }
    if (daftarMahasiswa.empty()) {
        cout << "Belum ada data mahasiswa.\n";
        return;
    }

    vector<Mahasiswa> acakMahasiswa = daftarMahasiswa;
    sort(acakMahasiswa.begin(), acakMahasiswa.end(), [](const Mahasiswa& a, const Mahasiswa& b) {
        return a.ipk > b.ipk;
    });

    random_device rd;
    mt19937 g(rd());
    shuffle(acakMahasiswa.begin() + (acakMahasiswa.size() / 3), acakMahasiswa.end(), g);

    riwayatPembagian.push_back(pembagianKelompok);
    pembagianKelompok.clear();

    for (size_t i = 0; i < acakMahasiswa.size(); ++i) {
        int kelompokKe = (i % jumlahKelompok) + 1;
        pembagianKelompok[kelompokKe].push_back(acakMahasiswa[i]);
    }

    cout << "\nHasil Pembagian Kelompok:\n";
    for (const auto& [kelompok, anggota] : pembagianKelompok) {
        cout << "\nKelompok " << kelompok << ":\n";
        cout << left << setw(5) << "No" << setw(20) << "Nama" << setw(15) << "NIM"
             << setw(20) << "Jurusan" << setw(5) << "IPK" << endl;
        cout << string(65, '-') << endl;
        for (size_t i = 0; i < anggota.size(); ++i) {
            cout << left << setw(5) << i + 1 << setw(20) << anggota[i].nama
                 << setw(15) << anggota[i].nim << setw(20) << anggota[i].jurusan
                 << setw(5) << fixed << setprecision(2) << anggota[i].ipk << endl;
        }
    }
    cout << "\033[1;32mPembagian berhasil dilakukan.\033[0m\n";
}

// Menampilkan hasil pembagian kelompok dalam format tabel
void lihatPembagian() {
    system(CLEAR);
    cout << "===== Hasil Pembagian Kelompok =====\n\n";
    if (pembagianKelompok.empty()) {
        cout << "Belum ada pembagian kelompok.\n";
        return;
    }
    for (const auto& [kelompok, anggota] : pembagianKelompok) {
        cout << "\nKelompok " << kelompok << ":\n";
        cout << left << setw(5) << "No" << setw(20) << "Nama" << setw(15) << "NIM"
             << setw(20) << "Jurusan" << setw(5) << "IPK" << endl;
        cout << string(65, '-') << endl;
        for (size_t i = 0; i < anggota.size(); ++i) {
            cout << left << setw(5) << i + 1 << setw(20) << anggota[i].nama
                 << setw(15) << anggota[i].nim << setw(20) << anggota[i].jurusan
                 << setw(5) << fixed << setprecision(2) << anggota[i].ipk << endl;
        }
    }
}

// Mengembalikan pembagian kelompok ke riwayat sebelumnya
void undoPembagian() {
    system(CLEAR);
    cout << "===== Undo Pembagian =====\n\n";
    if (riwayatPembagian.empty()) {
        cout << "Tidak ada riwayat untuk undo.\n";
        return;
    }
    pembagianKelompok = riwayatPembagian.back();
    riwayatPembagian.pop_back();
    cout << "\033[1;32mUndo berhasil.\033[0m\n";
}

// Menyimpan data mahasiswa ke file data_mahasiswa.txt
void simpanMahasiswa() {
    system(CLEAR);
    cout << "===== Simpan Data Mahasiswa =====\n\n";
    ofstream file("data_mahasiswa.txt", ios::out | ios::trunc);
    if (!file.is_open()) {
        cout << "Gagal membuka file untuk penulisan.\n";
        return;
    }
    for (const auto& m : daftarMahasiswa) {
        file << m.nama << ";" << m.nim << ";" << m.jurusan << ";" << m.ipk << "\n";
    }
    file.close();
    cout << "\033[1;32mData mahasiswa disimpan ke data_mahasiswa.txt.\033[0m\n";
}

// Mengekspor hasil pembagian kelompok ke file hasil_pembagian.txt
void exportKeFile() {
    system(CLEAR);
    cout << "===== Ekspor Pembagian ke File =====\n\n";
    if (ifstream("hasil_pembagian.txt")) {
        cout << "File sudah ada. Timpa? (y/n): ";
        char choice; cin >> choice;
        if (tolower(choice) != 'y') {
            cout << "Ekspor dibatalkan.\n";
            return;
        }
    }
    ofstream file("hasil_pembagian.txt", ios::out | ios::trunc);
    if (!file.is_open()) {
        cout << "Gagal membuka file untuk penulisan.\n";
        return;
    }
    for (const auto& [kelompok, anggota] : pembagianKelompok) {
        file << "Kelompok " << kelompok << ":\n";
        for (const auto& m : anggota) {
            file << "- " << m.nama << " (" << m.nim << ", IPK: "
                 << fixed << setprecision(2) << m.ipk << ")\n";
        }
    }
    file.close();
    cout << "\033[1;32mData berhasil diekspor ke hasil_pembagian.txt.\033[0m\n";
}

// Mereset semua data mahasiswa dan pembagian kelompok
void resetData() {
    system(CLEAR);
    cout << "===== Reset Semua Data =====\n\n";
    daftarMahasiswa.clear();
    pembagianKelompok.clear();
    riwayatPembagian.clear();
    cout << "\033[1;32mSemua data telah direset.\033[0m\n";
}

int main() {
    loadingScreen();
    system("color 07");
    system(CLEAR);
    int pilihan;
    do {
        cout << "=====================================\n";
        cout << "|          MAIN MENU APP            |\n";
        cout << "|         Team Builder Pro          |\n";
        cout << "|                                   |\n";
        cout << "|  Pilih opsi untuk mengelola data: |\n";
        cout << "=====================================\n";
        cout << "1. Tambah Mahasiswa\n";
        cout << "2. Tampilkan Semua Mahasiswa\n";
        cout << "3. Ubah Data Mahasiswa\n";
        cout << "4. Hapus Mahasiswa\n";
        cout << "5. Tampilkan Pot Berdasarkan IPK\n";
        cout << "6. Bagi Kelompok Secara Acak\n";
        cout << "7. Lihat Hasil Pembagian\n";
        cout << "8. Undo Pembagian\n";
        cout << "9. Simpan Data Mahasiswa\n";
        cout << "10. Export Pembagian ke File\n";
        cout << "11. Reset Semua Data\n";
        cout << "0. Keluar\n";
        cout << "=====================================\n";
        while (!inputNumerik(pilihan, "Pilih menu (0-11): ")) {
            cout << "Pilih menu (0-11): ";
        }

        system(CLEAR);
        switch (pilihan) {
            case 1: tambahMahasiswa(); waitForEnterAndClear(); break;
            case 2: tampilkanSemua(); waitForEnterAndClear(); break;
            case 3: ubahMahasiswa(); waitForEnterAndClear(); break;
            case 4: hapusMahasiswa(); waitForEnterAndClear(); break;
            case 5: tampilkanPot(); waitForEnterAndClear(); break;
            case 6: bagiKelompokAcak(); waitForEnterAndClear(); break;
            case 7: lihatPembagian(); waitForEnterAndClear(); break;
            case 8: undoPembagian(); waitForEnterAndClear(); break;
            case 9: simpanMahasiswa(); waitForEnterAndClear(); break;
            case 10: exportKeFile(); waitForEnterAndClear(); break;
            case 11: resetData(); waitForEnterAndClear(); break;
            case 0: cout << "\033[1;32mTerima kasih telah menggunakan program ini!\033[0m\n"; break;
            default: cout << "\033[1;32mPilihan tidak valid, Input rentang (0-11)!\033[0m\n"; waitForEnterAndClear();
        }
    } while (pilihan != 0);
    return 0;
}