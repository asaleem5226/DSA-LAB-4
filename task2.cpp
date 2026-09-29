#include <cstddef>
#include <iostream>
#include <limits>

using namespace std;

class CircularList {
private:
    struct Person {
        int id;
        Person* next;

        explicit Person(int personId)
            : id(personId), next(nullptr) {
        }
    };

    Person* head;
    Person* tail;
    size_t size;

public:
    CircularList() : head(nullptr), tail(nullptr), size(0) {
    }

    // Circle ke tamam nodes ki memory free karta hai.
    ~CircularList() {
        if (head == nullptr) {
            return;
        }

        Person* node = head->next;

        while (node != head) {
            Person* nextNode = node->next;
            delete node;
            node = nextNode;
        }

        delete head;
    }

    void createCircle(int peopleCount) {
        for (int id = 1; id <= peopleCount; ++id) {
            Person* newPerson = new Person(id);

            if (head == nullptr) {
                head = newPerson;
                tail = newPerson;
                newPerson->next = head;
            } else {
                newPerson->next = head;
                tail->next = newPerson;
                tail = newPerson;
            }

            ++size;
        }
    }

    void simulate(unsigned long long step) {
        Person* current = head;
        Person* previous = tail;
        bool firstElimination = true;

        cout << "Elimination order: ";

        while (size > 1) {
            // Current person ko count 1 samjha hai.
            // Is liye eliminate hone wale k-th person tak k-1 moves hain.
            size_t moves = static_cast<size_t>((step - 1) % size);

            for (size_t i = 0; i < moves; ++i) {
                previous = current;
                current = current->next;
            }

            if (!firstElimination) {
                cout << ", ";
            }

            cout << current->id;
            firstElimination = false;

            if (current == head) {
                head = current->next;
            }

            if (current == tail) {
                tail = previous;
            }

            // Previous node ko eliminated node ke next node se joro.
            previous->next = current->next;

            delete current;
            --size;

            // Counting eliminated person ke baad wale person se resume hoti hai.
            current = previous->next;
        }

        cout << "\nSurvivor: " << head->id << '\n';
    }
};

int main() {
    long long n;
    long long k;

    cout << "Enter number of people (N): ";
    if (!(cin >> n) || n <= 0 || n > numeric_limits<int>::max()) {
        cout << "N must be a positive integer within the supported range.\n";
        return 1;
    }

    cout << "Enter step count (k): ";
    if (!(cin >> k) || k <= 0) {
        cout << "k must be a positive integer.\n";
        return 1;
    }

    CircularList circle;
    circle.createCircle(static_cast<int>(n));
    circle.simulate(static_cast<unsigned long long>(k));

    return 0;
}