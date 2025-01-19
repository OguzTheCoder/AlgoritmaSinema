#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <locale.h>
#include <ctime>

using namespace std;
/*
Müşteri ekle çıkar || Ekle kısmı çalışıyor, çıkar kısmı bazen çalışmıyor (Bilgisayardan dolayı sanırım)
Koltuk seçimi yaptır, dolu koltukları göster || Çalışıyor
Her filme özel koltuk sınırı getir || Çalışıyor
Film özel saatlarini ayarla || Yapamadım
Film 3 boyutlu mu değil mi || Yapamadım
Filme özel yaş kısıtlaması ekle || Çalışıyor
Koltuk seçiminde kızın yanına erkek oturmasını engelle, erkeğin yanına kız oturmasına izin ver || Yapamadım
*/

/*
struct Film
struct Customer
struct Seat
*/

struct Film {
    string name; // Filmin adı
    string category; // Film Kategorisi
    bool is3D; // Müşterinin 3 boyutlu tercihi
    int ageRestriction; // Yaş kısıtlaması
    float duration; // Filmin süresi
};

struct Customer {
    string fullName; // Admin için
    int age; // Yaş kısıtlaması için
    char gender; // Koltuk seçimi için
    string selectedFilm; // Hangi filmi tercih etti
    string sessionTime; // Hangi saatteki film
    string seat; // Hangi koltuğu seçti (Filme özel)
    float price; // Seçtiği kriterlere uygun fiyat
};

//Bu kısmın tamamını ChatGPT'den kopyaladım. Vector kullanımını bilmiyordum. 
vector<Film> films = {
    {"Jigsaw", "Horror", true, 18, 120},
    {"Wicked", "Romance", false, 0, 130},
    {"Dictator", "Comedy", false, 0, 95},
    {"Star Wars", "Sci-Fi", true, 7, 140}
};


void listFilms();
void addCustomer();
void removeCustomer();
void displayMenu();
void selectSeat(const string& filmName, string& selectedSeat);

int main() {
    setlocale(LC_ALL, "Turkish"); //Visual Studio Code'da dil paketi çalışmıyor.
    int choice;
    do {
        displayMenu(); // Tercih
        cout << "What you want to do: ";
        cin >> choice;

        switch (choice) {
        case 1:
            listFilms();
            break;
        case 2:
            addCustomer();
            break;
        case 3:
            removeCustomer();
            break;
        case 0:
            cout << "Leaving the system..." << endl;
            break;
        default:
            cout << "Invalid choice! " << endl;
        }

    } while (choice != 0);

    return 0;
}


void listFilms() { // ChatGPT'den yardım aldım. Film listesini gösterir
    cout << "\n=== List of Films ===\n";
    for (size_t i = 0; i < films.size(); i++) {
        cout << i + 1 << ". " << films[i].name << " - " << films[i].category 
             << " (" << (films[i].is3D ? "3D" : "2D") << ") - Time: " 
             << films[i].duration << " min - Age restriction: "
             << (films[i].ageRestriction > 0 ? to_string(films[i].ageRestriction) : "None") << endl;
    }
}


void addCustomer() { // Müşteriyi dosyalar ve Customer structure'ında yeni müşteri ekler
    Customer newCustomer;
    int selectedFilm;
    ofstream customerInfo("customers.txt", ios::app); // Dosyalama kısmı

    if (!customerInfo) {
        cerr << "Error opening customers file!" << endl;
        return;
    }

    cout << "\n=== Adding a New Customer ===" << endl;

    cin.ignore(); // <-- GPT'den çaldım, bunsuz kod çalışmıyor
    cout << "Full Name: ";
    getline(cin, newCustomer.fullName);

    cout << "Age: ";
    cin >> newCustomer.age;

    cout << "Gender (E/K): ";
    cin >> newCustomer.gender;

    cout << "\nPlease choose the film you wanna watch: " << endl;
    listFilms(); // Film listesi, hangi saatlerde hangi filmler var

    cout << "Film number: ";
    cin >> selectedFilm;

    if (selectedFilm < 1 || selectedFilm > films.size()) {
        cout << "Invalid choice!" << endl;
        return;
    }

    
    newCustomer.selectedFilm = films[selectedFilm - 1].name; // Müşterinin hangi filmi seçtiğini kaydeder

    cout << "Session time (For instance: 15:30): ";
    cin >> newCustomer.sessionTime; // Müşterinin hangi saati tercih ettiğini kaydeder


    if (newCustomer.age < films[selectedFilm - 1].ageRestriction) { // Seçtiği filme yaşı yetiyor mu, yetmiyorsa sistemden at
        cout << "Age restricted film\n Gettin back to the system..." << endl;
        return;
    }

    
    selectSeat(films[selectedFilm - 1].name, newCustomer.seat); // Müşteri hangi koltuğu tercih etti


    newCustomer.price = films[selectedFilm - 1].is3D ? 50.0 : 40.0; // Fiyat, GPT'den aldım

    // Müşterinin seçtiği kriterlere göre özel dosyalama yapıyor
    customerInfo << "Full Name: " << newCustomer.fullName << endl;
    customerInfo << "Age: " << newCustomer.age << endl;
    customerInfo << "Gender: " << newCustomer.gender << endl;
    customerInfo << "Selected Film: " << newCustomer.selectedFilm << endl;
    customerInfo << "Session Time: " << newCustomer.sessionTime << endl;
    customerInfo << "Seat: " << newCustomer.seat << endl;
    customerInfo << "Price: " << newCustomer.price << " TL" << endl;
    customerInfo << "--------------------------" << endl;

    customerInfo.close();

    cout << "\n" << newCustomer.fullName << " has been added to the list!" << endl;
}

// GPT, koltuk seçimini yapan fonksiyon
void selectSeat(const string& filmName, string& selectedSeat) {
    string seatFileName = filmName + "Seats.txt";
    ifstream seatFile(seatFileName);
    ofstream tempFile;
    set<string> occupiedSeats;
    string seat;

    // GPT, daha önceden kapılmış koltukları gösterir(?)
    if (seatFile) {
        while (seatFile >> seat) {
            occupiedSeats.insert(seat);
        }
        seatFile.close();
    }

    // GPT, mevcut koltukları gösterir
    cout << "\nAvailable seats for " << filmName << ":\n";
    for (char row = 'A'; row <= 'H'; row++) {
        for (int col = 1; col <= 8; col++) {
            string currentSeat = string(1, row) + to_string(col);
            if (occupiedSeats.find(currentSeat) == occupiedSeats.end()) {
                cout << currentSeat << " ";
            } else {
                cout << "XX ";
            }
        }
        cout << endl;
    }

    
    cout << "Choose your seat (For instance, A4): ";
    cin >> selectedSeat; // Koltuk seçiimi

    if (occupiedSeats.find(selectedSeat) != occupiedSeats.end()) { // GPT, Koltuk dolu mu değil mi
        cout << "Seat is already occupied. Please choose another seat.\n";
        selectSeat(filmName, selectedSeat); // GPT, terkar seçim yaptırır
        return;
    }

    // Koltuk seçimini dosyalar
    tempFile.open(seatFileName, ios::app);
    if (tempFile) {
        tempFile << selectedSeat << endl;
        tempFile.close();
    } else {
        cerr << "Error updating seat file!" << endl;
    }
}

// %90'ını GPT yaptı. GPT'siz yapmayı denedim, Structure içinden silebildim ancak dosya içerisinden silemedim.
void removeCustomer() {
    string customerName;
    bool found = false;
    vector<Customer> customerList; // Temporary list to hold all customers (GPT)
    ifstream inFile("customers.txt"); // Input file for customer data (GPT)
    ofstream customerInfo("customers_temp.txt"); // Temporary file for updated customer data (GPT)

    if (!inFile || !customerInfo) {
        cerr << "Error: Unable to access customer files!" << endl;
        return;
    }

    cout << "\nEnter the full name of the customer you want to remove: ";
    cin.ignore();
    getline(cin, customerName);

    string fullName, selectedFilm, sessionTime, ageStr, genderStr, priceStr;
    while (getline(inFile, fullName)) { // Read each customer record (GPT)
        getline(inFile, ageStr); // Skip age (GPT)
        getline(inFile, genderStr); // Skip gender (GPT)
        getline(inFile, selectedFilm); // Selected Film (GPT)
        getline(inFile, sessionTime); // Session Time (GPT)
        getline(inFile, priceStr); // Price (GPT)
        inFile.ignore(); // Skip the separator line (GPT)

        // Check if this is the customer to be removed (GPT)
        if (fullName.substr(10) == customerName) {
            found = true;

            // Remove the seat reservation from the film's seat file (GPT)
            string filmName = selectedFilm.substr(15); // Extract film name (GPT)
            string seatFileName = filmName + "Seats.txt";
            string session = sessionTime.substr(14); // Extract session time (GPT)

            cout << "Customer found: " << customerName << endl;
            cout << "Removing seat reservation for film: " << filmName << " (" << session << ")" << endl;

            // Read seat file and remove the specific seat (GPT)
            vector<string> seats;
            string seat;
            ifstream seatFile(seatFileName);
            ofstream tempSeatFile(seatFileName + "_temp");

            if (seatFile && tempSeatFile) {
                while (getline(seatFile, seat)) {
                    if (seat != session) { // Keep all seats except the one to be removed (GPT)
                        tempSeatFile << seat << endl;
                    }
                }
                seatFile.close();
                tempSeatFile.close();
                remove(seatFileName.c_str()); // Delete old seat file (GPT)
                rename((seatFileName + "_temp").c_str(), seatFileName.c_str()); // Rename temp to original (GPT)
            } else {
                cerr << "Error: Unable to update seat file for " << filmName << endl;
            }
        } else {
            // Write the customer back to the temp file if they are not removed (GPT)
            customerInfo << fullName << endl;
            customerInfo << ageStr << endl;
            customerInfo << genderStr << endl;
            customerInfo << selectedFilm << endl;
            customerInfo << sessionTime << endl;
            customerInfo << priceStr << endl;
            customerInfo << "--------------------------" << endl;
        }
    }

    inFile.close();
    customerInfo.close();

    if (found) {
        // Replace the old customers file with the updated one (GPT)
        remove("customers.txt");
        rename("customers_temp.txt", "customers.txt");
        cout << "Customer and their reservation have been successfully removed!" << endl;
    } else {
        cout << "Customer not found!" << endl;
        remove("customers_temp.txt"); // Remove temp file if no changes made (GPT)
    }
}

void displayMenu() { // Menü ekranı
    cout << "\n=== Sinema Otomasyonu ===\n";
    cout << "1. Show the list of films\n";
    cout << "2. Add a customer\n";
    cout << "3. Remove a customer\n";
    cout << "0. Exit\n";
    cout << "=========================" << endl;
}
