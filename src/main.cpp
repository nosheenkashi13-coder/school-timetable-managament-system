#include <iostream>
#include <string>
using namespace std;

// =====================
// STRUCTURES
// =====================
struct Teacher {
    int id;
    string name;
    string subject;
    string availability;
    Teacher* next;
};

struct ClassRoom {
    string className;
    string subject;
    ClassRoom* next;
};

struct Schedule {
    string teacherName;
    string className;
    string subject;
    Schedule* next;
};

// Queue for classes (FIFO)
struct QueueClass {
    string className;
    string subject;
    QueueClass* next;
};

// Stack for Undo last schedule (LIFO)
struct UndoStack {
    string teacherName;
    string className;
    string subject;
    UndoStack* next;
};

// =====================
// GLOBAL POINTERS
// =====================
Teacher* headTeacher = NULL;
ClassRoom* headClass = NULL;
Schedule* headSchedule = NULL;
QueueClass* frontQ = NULL;
QueueClass* rearQ = NULL;
UndoStack* topUndo = NULL;

// =====================
// TEACHER MANAGEMENT
// =====================
void addTeacher() {
    Teacher* newT = new Teacher;
    cout << "\nEnter Teacher ID: ";
    cin >> newT->id;
    cout << "Enter Name: ";
    cin >> newT->name;
    cout << "Enter Subject: ";
    cin >> newT->subject;
    cout << "Enter Availability: ";
    cin >> newT->availability;
    newT->next = NULL;

    if (headTeacher == NULL) headTeacher = newT;
    else {
        Teacher* temp = headTeacher;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newT;
    }

    cout << "? Teacher added successfully.\n";
}

void viewTeachers() {
    if (headTeacher == NULL) {
        cout << "? No teachers found.\n";
        return;
    }
    cout << "\n?? Teacher List:\n";
    Teacher* temp = headTeacher;
    while (temp != NULL) {
        cout << "ID: " << temp->id << " | Name: " << temp->name
             << " | Subject: " << temp->subject
             << " | Availability: " << temp->availability << endl;
        temp = temp->next;
    }
}

// =====================
// CLASS MANAGEMENT WITH QUEUE
// =====================
void enqueueClass(string cname, string sub) {
    QueueClass* newNode = new QueueClass;
    newNode->className = cname;
    newNode->subject = sub;
    newNode->next = NULL;

    if (rearQ == NULL) {
        frontQ = rearQ = newNode;
    } else {
        rearQ->next = newNode;
        rearQ = newNode;
    }
}

bool dequeueClass(string &cname, string &sub) {
    if (frontQ == NULL) return false;
    QueueClass* temp = frontQ;
    cname = temp->className;
    sub = temp->subject;
    frontQ = frontQ->next;
    if (frontQ == NULL) rearQ = NULL;
    delete temp;
    return true;
}

void addClass() {
    ClassRoom* newC = new ClassRoom;
    cout << "\nEnter Class Name: ";
    cin >> newC->className;
    cout << "Enter Subject: ";
    cin >> newC->subject;
    newC->next = NULL;

    enqueueClass(newC->className, newC->subject);

    if (headClass == NULL) headClass = newC;
    else {
        ClassRoom* temp = headClass;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newC;
    }
    cout << "? Class added successfully.\n";
}

void viewClasses() {
    if (headClass == NULL) {
        cout << "? No classes found.\n";
        return;
    }
    cout << "\n?? Classes List:\n";
    ClassRoom* temp = headClass;
    while (temp != NULL) {
        cout << "Class: " << temp->className << " | Subject: " << temp->subject << endl;
        temp = temp->next;
    }
}

// =====================
// STACK FOR UNDO
// =====================
void pushUndo(string t, string c, string s) {
    UndoStack* newNode = new UndoStack;
    newNode->teacherName = t;
    newNode->className = c;
    newNode->subject = s;
    newNode->next = topUndo;
    topUndo = newNode;
}

bool popUndo(string &t, string &c, string &s) {
    if (topUndo == NULL) return false;
    UndoStack* temp = topUndo;
    t = temp->teacherName;
    c = temp->className;
    s = temp->subject;
    topUndo = topUndo->next;
    delete temp;
    return true;
}

// =====================
// SCHEDULE GENERATOR
// =====================
void generateSchedule() {
    if (headTeacher == NULL || frontQ == NULL) {
        cout << "? Need teachers and classes to generate schedule.\n";
        return;
    }

    Teacher* tempTeacher = headTeacher;
    string cname, sub;

    cout << "\n?? Generating Schedule...\n";

    while (dequeueClass(cname, sub)) {
        Schedule* newS = new Schedule;
        newS->teacherName = tempTeacher->name;
        newS->className = cname;
        newS->subject = sub;
        newS->next = headSchedule;
        headSchedule = newS;

        pushUndo(newS->teacherName, newS->className, newS->subject);

        cout << "? " << newS->teacherName << " assigned to " << newS->className 
             << " (" << newS->subject << ")\n";

        tempTeacher = tempTeacher->next;
        if (tempTeacher == NULL) tempTeacher = headTeacher;
    }

    cout << "\n? Schedule generated successfully!\n";
}

void undoLastAssignment() {
    string t, c, s;
    if (!popUndo(t, c, s)) {
        cout << "? Nothing to undo.\n";
        return;
    }

    Schedule* temp = headSchedule;
    Schedule* prev = NULL;
    while (temp != NULL) {
        if (temp->teacherName == t && temp->className == c && temp->subject == s) {
            if (prev == NULL) headSchedule = temp->next;
            else prev->next = temp->next;
            delete temp;
            cout << "?? Undo successful! Last assignment removed.\n";
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

// =====================
// VIEW TIMETABLE
// =====================
void viewTimetable() {
    if (headSchedule == NULL) {
        cout << "? Timetable empty.\n";
        return;
    }
    cout << "\n?? Timetable:\n";
    Schedule* temp = headSchedule;
    while (temp != NULL) {
        cout << "Teacher: " << temp->teacherName << " | Class: " << temp->className
             << " | Subject: " << temp->subject << endl;
        temp = temp->next;
    }
}

// =====================
// MENUS
// =====================
void teacherMenu() {
    int choice;
    do {
        cout << "\n===== Teacher Menu =====";
        cout << "\n1. Add Teacher";
        cout << "\n2. View Teachers";
        cout << "\n0. Back";
        cout << "\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addTeacher(); break;
            case 2: viewTeachers(); break;
        }
    } while (choice != 0);
}

void classMenu() {
    int choice;
    do {
        cout << "\n===== Class Menu =====";
        cout << "\n1. Add Class";
        cout << "\n2. View Classes";
        cout << "\n0. Back";
        cout << "\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addClass(); break;
            case 2: viewClasses(); break;
        }
    } while (choice != 0);
}

void scheduleMenu() {
    int choice;
    do {
        cout << "\n===== Schedule Menu =====";
        cout << "\n1. Generate Schedule";
        cout << "\n2. View Timetable";
        cout << "\n3. Undo Last Assignment";
        cout << "\n0. Back";
        cout << "\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: generateSchedule(); break;
            case 2: viewTimetable(); break;
            case 3: undoLastAssignment(); break;
        }
    } while (choice != 0);
}

// =====================
// MAIN MENU
// =====================
int main() {
    int choice;
    do {
        cout << "\n===============================";
        cout << "\n  SCHOOL TIMETABLE SYSTEM";
        cout << "\n===============================";
        cout << "\n1. Teacher Management";
        cout << "\n2. Class Management";
        cout << "\n3. Schedule Generator";
        cout << "\n4. View Timetable";
        cout << "\n0. Exit";
        cout << "\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: teacherMenu(); break;
            case 2: classMenu(); break;
            case 3: scheduleMenu(); break;
            case 4: viewTimetable(); break;
            case 0: cout << "? Exiting...\n"; break;
            default: cout << "? Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
