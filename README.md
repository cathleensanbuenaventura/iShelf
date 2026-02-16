# iShelf: A Library Management System (HinLIBS)

A Qt-based library management system application with SQLite persistence, built for Object-Oriented Software Engineering.

---

## Tech Stack

| Tool | Purpose |
|------|---------|
| C++17 | Core application logic |
| Qt 5/6 (Widgets, SQL) | GUI framework |
| SQLite (`hinlibs.sqlite3`) | Persistent data storage |
| Qt Creator | IDE / build system |

---

## Features

### Patron
- Browse and search the catalogue (20 pre-loaded items)
- Borrow items — max 3 active loans, 14-day loan period
- Return items — triggers hold notifications automatically
- Place and cancel holds (FIFO queue)
- View account status — current loans with due dates, hold queue positions

### Librarian
- Add a new item to the catalogue (supports all item types)
- Remove an item — blocked if currently checked out or has active holds
- Return an item on behalf of a patron by searching their account

### Administrator
- Login supported; extended functionality not implemented in D2

---

## Prerequisites

- Qt Creator with Qt 5.12+ or Qt 6 (including the `sql` module)
- The course Ubuntu VM already has the required Qt kit configured

---

## Setup & Build

> **Note:** The database path is hardcoded to `/home/student/Team_50_D2/Team_50_D2/hinlibs.sqlite3`.
> The project folder must be placed at exactly that location on the VM.

```
/home/student/
└── Team_50_D2/
    └── Team_50_D2/       ← clone/place this repo here
        ├── 3004-Project.pro
        ├── hinlibs.sqlite3
        ├── src/
        └── ...
```

1. Open Qt Creator.
2. **File → Open File or Project…** → select `3004-Project.pro`.
3. Accept the default Qt kit and click **Configure Project**.
4. Click the green **Run** button (or **Build → Build Project**, then **Run**).

The compiled binary is output to `build/bin/HinLIBS`.

---

## Login Accounts

All usernames are case-insensitive. No password is required.

| Username | Role |
|----------|------|
| `alice`, `bob`, `carol`, `dave`, `erin` | Patron |
| `libby` | Librarian |
| `admin` | Administrator |

---

## Database

`hinlibs.sqlite3` ships pre-populated with the project:

- **20 catalogue items** — 5 fiction books, 5 non-fiction books, 3 magazines, 3 movies, 4 video games
- **7 users** — 5 patrons, 1 librarian, 1 admin
- All loans and holds persist across sessions; restarting reloads the same state

---