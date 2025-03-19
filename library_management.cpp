#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <typeinfo>

using namespace std;

class LibraryItem {
protected:
    string title;
    int id;
public:
    LibraryItem(const string& t = "", int i = 0) : title(t), id(i) {}
    virtual void display() const {
        cout << "ID: " << id << ", Title: " << title << endl;
    }
    virtual void saveToFile(ofstream& outFile) const {
        outFile << typeid(*this).name() << endl;
        outFile << id << endl;
        outFile << title << endl;
    }
    virtual void loadFromFile(ifstream& inFile) {
        inFile >> id;
        inFile.ignore();
        getline(inFile, title);
    }
    virtual ~LibraryItem() {}
};

class Book : public LibraryItem {
    string author;
public:
    Book(const string& t = "", int i = 0, const string& a = "")
        : LibraryItem(t, i), author(a) {}
    void display() const {
        cout << "Book - ";
        LibraryItem::display();
        cout << "Author: " << author << endl;
    }
    void readSample() const {
        cout << "Reading sample of book: " << title << endl;
    }
    void saveToFile(ofstream& outFile) const {
        LibraryItem::saveToFile(outFile);
        outFile << author << endl;
    }
    void loadFromFile(ifstream& inFile) {
        LibraryItem::loadFromFile(inFile);
        getline(inFile, author);
    }
};

void showItemDetails(const LibraryItem& item) {
    item.display();
}

void performSpecificAction(LibraryItem* item) {
    if (Book* book = dynamic_cast<Book*>(item)) {
        book->readSample();
    } else {
        cout << "Unknown item type." << endl;
    }
}

void saveLibraryToFile(const vector<LibraryItem*>& library, const string& filename) {
    ofstream outFile(filename.c_str());
    if (!outFile) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }
    outFile << library.size() << endl;
    for (vector<LibraryItem*>::const_iterator it = library.begin(); it != library.end(); ++it) {
        (*it)->saveToFile(outFile);
    }
    outFile.close();
}

void loadLibraryFromFile(vector<LibraryItem*>& library, const string& filename) {
    ifstream inFile(filename.c_str());
    if (!inFile) {
        cerr << "Error opening file for reading: " << filename << endl;
        return;
    }
    size_t librarySize;
    inFile >> librarySize;
    inFile.ignore();
    for (size_t i = 0; i < librarySize; ++i) {
        string typeName;
        getline(inFile, typeName);
        LibraryItem* item = NULL;
        if (typeName.find("Book") != string::npos) {
            item = new Book();
        }
        if (item) {
            item->loadFromFile(inFile);
            library.push_back(item);
        }
    }
    inFile.close();
}

int main() {
    vector<LibraryItem*> library;
    loadLibraryFromFile(library, "library.txt");

    library.push_back(new Book("The Great Gatsby", 1, "F. Scott Fitzgerald"));

    for (vector<LibraryItem*>::iterator it = library.begin(); it != library.end(); ++it) {
        showItemDetails(**it);
        performSpecificAction(*it);
        cout << endl;
    }

    saveLibraryToFile(library, "library.txt");

    for (vector<LibraryItem*>::iterator it = library.begin(); it != library.end(); ++it) {
        delete *it;
    }

    return 0;
}