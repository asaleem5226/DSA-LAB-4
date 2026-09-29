#include <iostream>
#include <limits>
#include <string>
#include <utility>

using namespace std;

class Playlist {
private:
    struct Song {
        int id;
        string name;
        string duration;
        Song* next;
        Song* previous;

        Song(int songId, const string& songName, const string& songDuration)
            : id(songId),
              name(songName),
              duration(songDuration),
              next(nullptr),
              previous(nullptr) {
        }
    };

    Song* head;
    Song* tail;
    Song* current;

    Song* findSong(int id) const {
        Song* node = head;

        while (node != nullptr) {
            if (node->id == id) {
                return node;
            }
            node = node->next;
        }

        return nullptr;
    }

    void printSong(const Song* song) const {
        cout << "ID: " << song->id
             << " | Name: " << song->name
             << " | Duration: " << song->duration;

        if (song == current) {
            cout << "  <-- Current song";
        }

        cout << '\n';
    }

public:
    Playlist() : head(nullptr), tail(nullptr), current(nullptr) {
    }

    // Playlist destroy hone par tamam nodes ki memory free hoti hai.
    ~Playlist() {
        Song* node = head;

        while (node != nullptr) {
            Song* nextNode = node->next;
            delete node;
            node = nextNode;
        }
    }

    void addSong(int id, const string& name, const string& duration) {
        if (findSong(id) != nullptr) {
            cout << "This song ID already exists. Use a different ID.\n";
            return;
        }

        Song* newSong = new Song(id, name, duration);

        if (head == nullptr) {
            head = newSong;
            tail = newSong;
        } else {
            newSong->previous = tail;
            tail->next = newSong;
            tail = newSong;
        }

        cout << "Song added successfully.\n";
    }

    void deleteSong(int id) {
        Song* song = findSong(id);

        if (song == nullptr) {
            cout << "Song not found.\n";
            return;
        }

        if (song->previous != nullptr) {
            song->previous->next = song->next;
        } else {
            head = song->next;
        }

        if (song->next != nullptr) {
            song->next->previous = song->previous;
        } else {
            tail = song->previous;
        }

        if (current == song) {
            current = (song->next != nullptr) ? song->next : song->previous;
        }

        delete song;
        cout << "Song deleted successfully.\n";
    }

    void displayForward() const {
        if (head == nullptr) {
            cout << "The playlist is empty.\n";
            return;
        }

        cout << "\nPlaylist: first to last\n";
        Song* node = head;

        while (node != nullptr) {
            printSong(node);
            node = node->next;
        }
    }

    void displayBackward() const {
        if (tail == nullptr) {
            cout << "The playlist is empty.\n";
            return;
        }

        cout << "\nPlaylist: last to first\n";
        Song* node = tail;

        while (node != nullptr) {
            printSong(node);
            node = node->previous;
        }
    }

    void searchSong(int id) const {
        Song* song = findSong(id);

        if (song == nullptr) {
            cout << "Song not found.\n";
            return;
        }

        cout << "Song found:\n";
        printSong(song);
    }

    void playNext() {
        if (head == nullptr) {
            cout << "The playlist is empty.\n";
            return;
        }

        if (current == nullptr) {
            current = head;
        } else if (current->next != nullptr) {
            current = current->next;
        } else {
            cout << "This is already the last song.\n";
            return;
        }

        cout << "Now playing:\n";
        printSong(current);
    }

    void playPrevious() {
        if (tail == nullptr) {
            cout << "The playlist is empty.\n";
            return;
        }

        if (current == nullptr) {
            current = tail;
        } else if (current->previous != nullptr) {
            current = current->previous;
        } else {
            cout << "This is already the first song.\n";
            return;
        }

        cout << "Now playing:\n";
        printSong(current);
    }

    void reversePlaylist() {
        if (head == nullptr) {
            cout << "The playlist is empty.\n";
            return;
        }

        Song* node = head;

        while (node != nullptr) {
            Song* oldNext = node->next;

            node->next = node->previous;
            node->previous = oldNext;

            node = oldNext;
        }

        swap(head, tail);
        cout << "Playlist reversed successfully.\n";
    }
};

int main() {
    Playlist playlist;

    while (true) {
        cout << "\n========== Playlist Menu ==========\n";
        cout << "1. Add song\n";
        cout << "2. Delete song by ID\n";
        cout << "3. Display playlist forward\n";
        cout << "4. Display playlist backward\n";
        cout << "5. Search song by ID\n";
        cout << "6. Play next song\n";
        cout << "7. Play previous song\n";
        cout << "8. Reverse playlist\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        int choice;

        if (!(cin >> choice)) {
            cout << "Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 0) {
            cout << "Exiting playlist program.\n";
            break;
        }

        if (choice == 1) {
            int id;
            string name;
            string duration;

            cout << "Enter song ID: ";
            if (!(cin >> id)) {
                cout << "Invalid ID.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter song name: ";
            getline(cin, name);

            cout << "Enter duration (example: 3:45): ";
            getline(cin, duration);

            playlist.addSong(id, name, duration);
        } else if (choice == 2) {
            int id;
            cout << "Enter song ID to delete: ";

            if (!(cin >> id)) {
                cout << "Invalid ID.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            playlist.deleteSong(id);
        } else if (choice == 3) {
            playlist.displayForward();
        } else if (choice == 4) {
            playlist.displayBackward();
        } else if (choice == 5) {
            int id;
            cout << "Enter song ID to search: ";

            if (!(cin >> id)) {
                cout << "Invalid ID.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            playlist.searchSong(id);
        } else if (choice == 6) {
            playlist.playNext();
        } else if (choice == 7) {
            playlist.playPrevious();
        } else if (choice == 8) {
            playlist.reversePlaylist();
        } else {
            cout << "Invalid choice. Choose a number from the menu.\n";
        }
    }

    return 0;
}