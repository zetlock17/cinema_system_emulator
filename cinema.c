#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>
#include <windows.h>

// Добавить проверки логина и пароля при регистрации
// логин от 3 до 20 символов латиницы и цифр
// пароль от 6 до 20 символов литиница, минимум одна цифра, верхний и нижний регистр
// название фильма от 1 до 40 символов
// страна от 1 до 20 символов и с большой буквы

// добавить личный кабинет, где можно будет изменить свои данные 

// удаление фильма с админки должно удалять этот фильм у всех из списка избранного 


typedef struct Movie {
    char title[100];
    int year;
    char country[100];
    char genre[100];
    float rating;
    struct Movie* next;
    struct Movie* prev;
} Movie;

typedef struct User {
    char username[100];
    char password[100];
    char card[20];
    int favoriteCount;
    int isAdmin;
} User;

User currentUser;

Movie* createMovie(const char* title, int year, const char* country, const char* genre, float rating) {
    Movie* newMovie = (Movie*)malloc(sizeof(Movie));
    strcpy(newMovie->title, title);
    newMovie->year = year;
    strcpy(newMovie->country, country);
    strcpy(newMovie->genre, genre);
    newMovie->rating = rating;
    newMovie->next = NULL;
    newMovie->prev = NULL;
    return newMovie;
}

void addMovie(Movie** head, Movie* newMovie) {
    if (*head == NULL) {
        *head = newMovie;
        newMovie->next = newMovie;
        newMovie->prev = newMovie;
    } else {
        Movie* tail = (*head)->prev;
        tail->next = newMovie;
        newMovie->prev = tail;
        newMovie->next = *head;
        (*head)->prev = newMovie;
    }
}

void removeMovie(Movie** head, Movie* movie) {
    if (*head == NULL || movie == NULL) return;

    if (*head == movie && (*head)->next == *head) {
        *head = NULL;
    } else {
        Movie* prev = movie->prev;
        Movie* next = movie->next;
        prev->next = next;
        next->prev = prev;
        if (*head == movie) {
            *head = next;
        }
    }
    free(movie);
}

void saveMoviesToFile(Movie* head) {
    FILE* file = fopen("films.txt", "w");
    if (file == NULL) {
        perror("Unable to open films file");
        return;
    }

    Movie* current = head;
    do {
        fprintf(file, "%s\n%d\n%s\n%s\n%f\n", current->title, current->year, current->country, current->genre, current->rating);
        current = current->next;
    } while (current != head);

    fclose(file);
}

char* stringCreator(const char* string, int size) {
    char* result = (char*)malloc((size*2 + 1) * sizeof(char));
    int byteIndex = 0;
    int charCount = 0;
    int resultIndex = 0;

    while (string[byteIndex] != '\0' && charCount < size) {
        unsigned char c = (unsigned char)string[byteIndex];
        int charSize = 1;

        if (c >= 0xC0) {
            charSize = 2;
        }

        if (charCount + 1 <= size) {
            for (int i = 0; i < charSize; i++) {
                if (string[byteIndex] == '\0') break;
                result[resultIndex++] = string[byteIndex++];
            }
            charCount++;
        } else {
            break;
        }
    }

    while (charCount < size) {
        result[resultIndex++] = ' ';
        charCount++;
    }

    result[resultIndex] = '\0';
    return result;
}

int isFavorite(Movie* movie) {
    char filename[120];
    snprintf(filename, sizeof(filename), "favorites_%s.txt", currentUser.username);
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    char title[100];
    int year;
    char country[100];
    char genre[100];
    float rating;
    char buffer[100];

    while (fgets(title, sizeof(title), file)) {
        title[strcspn(title, "\n")] = '\0';
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "%d", &year);
        fgets(country, sizeof(country), file);
        country[strcspn(country, "\n")] = '\0';
        fgets(genre, sizeof(genre), file);
        genre[strcspn(genre, "\n")] = '\0';
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "%f", &rating);

        if (strcmp(title, movie->title) == 0 && year == movie->year && strcmp(country, movie->country) == 0 && strcmp(genre, movie->genre) == 0 && rating == movie->rating) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void updateUserFile() {
    FILE* file = fopen("users.txt", "r");
    if (file == NULL) {
        perror("Unable to open users file");
        return;
    }

    FILE* tempFile = fopen("temp_users.txt", "w");
    if (tempFile == NULL) {
        perror("Unable to open temp file");
        fclose(file);
        return;
    }

    char username[100];
    char password[100];
    char card[20];
    int favoriteCount;
    int isAdmin;

    while (fscanf(file, "%s %s %s %d %d", username, password, card, &favoriteCount, &isAdmin) != EOF) {
        if (strcmp(username, currentUser.username) == 0) {
            fprintf(tempFile, "%s %s %s %d %d\n", currentUser.username, currentUser.password, currentUser.card, currentUser.favoriteCount, currentUser.isAdmin);
        } else {
            fprintf(tempFile, "%s %s %s %d %d\n", username, password, card, favoriteCount, isAdmin);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("users.txt");
    rename("temp_users.txt", "users.txt");
}

void saveFavoriteMovie(Movie* movie) {
    if (isFavorite(movie)) {
        printf("Movie is already in favorites!\n");
        return;
    }

    char filename[120];
    snprintf(filename, sizeof(filename), "favorites_%s.txt", currentUser.username);
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        perror("Unable to open favorites file");
        return;
    }
    fprintf(file, "%s\n%d\n%s\n%s\n%f\n", movie->title, movie->year, movie->country, movie->genre, movie->rating);
    fclose(file);
    currentUser.favoriteCount++;
    updateUserFile();
}

void removeFavoriteMovie(Movie* movie) {
    char filename[120];
    snprintf(filename, sizeof(filename), "favorites_%s.txt", currentUser.username);
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open favorites file");
        return;
    }

    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("Unable to open temp file");
        fclose(file);
        return;
    }

    char title[100];
    int year;
    char country[100];
    char genre[100];
    float rating;
    char buffer[100];

    while (fgets(title, sizeof(title), file)) {
        title[strcspn(title, "\n")] = '\0';
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "%d", &year);
        fgets(country, sizeof(country), file);
        country[strcspn(country, "\n")] = '\0';
        fgets(genre, sizeof(genre), file);
        genre[strcspn(genre, "\n")] = '\0';
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "%f", &rating);

        if (strcmp(title, movie->title) != 0 || year != movie->year || strcmp(country, movie->country) != 0 || strcmp(genre, movie->genre) != 0 || rating != movie->rating) {
            fprintf(tempFile, "%s\n%d\n%s\n%s\n%f\n", title, year, country, genre, rating);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename("temp.txt", filename);
    currentUser.favoriteCount--;
    updateUserFile();
}

int validateCard(const char* card) {
    int len = strlen(card);
    if (len != 16) return 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(card[i])) return 0;
    }
    return 1;
}

void displayMovies(Movie* head, int isFavoriteList, const char* header) {
    if (head == NULL) {
        if (isFavoriteList) {
            printf("The favorites list is empty.\n");
            printf("Press any key to return to the menu...");
            _getch();
        }
        return;
    }

    Movie* current = head;
    while (1) {
        system("cls");
        
        // === Фильмы === (51 пробел) === Избранное === (49 пробелов)
        if (header == "Фильмы") {
            printf("                                                   === Фильмы ===\n");
        } else if (header == "Избранное") {
            printf("                                                 === Избранное ===\n");
        }

        char* prevTitle = stringCreator(current->prev->title, 30);
        char* currentTitle = stringCreator(current->title, 40);
        char* nextTitle = stringCreator(current->next->title, 30);
        char* currentCountry = stringCreator(current->country, 32);
        char* currentGenre = stringCreator(current->genre, 34);

        printf("+--------------------------------+  +------------------------------------------+  +--------------------------------+\n");
        printf("| %s |  ", prevTitle);
        printf("| %s |  ", currentTitle);
        printf("| %s |\n", nextTitle);
        printf("| Год: %-25d |  ", current->prev->year);
        printf("| Год: %-35d |  ", current->year);
        printf("| Год: %-25d |\n", current->next->year);
        printf("+--------------------------------+  | Страна: %s |  +--------------------------------+\n", currentCountry);
        printf("                                    ");
        printf("| Жанр: %s |\n", currentGenre);
        printf("                                    ");
        printf("| Рейтинг: %-31.1f |\n", current->rating);
        if (isFavorite(current)) {
            printf("                                    | В избранном: Да                          |\n");
        } else {
            printf("                                    | В избранном: Нет                         |\n");
        }
        printf("                                    +------------------------------------------+\n");

        free(prevTitle);
        free(currentTitle);
        free(nextTitle);
        free(currentCountry);
        free(currentGenre);

        printf("\n q - выход в меню, ");
        if (isFavoriteList) {
            printf("r - удалить из избранного");
        } else {
            printf("f - добавить в список избранных, r - удалить из избранного");
            if (currentUser.isAdmin) {
                printf(", g - удалить фильм");
            }
        }
        printf("\n");

        char ch = _getch();
        if (ch == 'a') {
            current = current->prev;
        } else if (ch == 'd') {
            current = current->next;
        } else if (ch == 'f' && !isFavoriteList) {
            saveFavoriteMovie(current);
        } else if (ch == 'r') {
            removeFavoriteMovie(current);
        } else if (ch == 'g' && currentUser.isAdmin) {
            removeMovie(&head, current);
            saveMoviesToFile(head);
            break;
        } else if (ch == 'q') {
            break;
        }
    }
}

void freeMovies(Movie* head) {
    if (head == NULL) return;
    Movie* current = head;
    do {
        Movie* temp = current;
        current = current->next;
        free(temp);
    } while (current != head);
}

int registerUser(const char* username, const char* password, const char* card) {
    FILE* file = fopen("users.txt", "a");
    if (file == NULL) {
        perror("Unable to open users file");
        return 0;
    }
    fprintf(file, "%s %s %s 0 0\n", username, password, card); // 0 0 means not admin and 0 favorite movies
    fclose(file);
    return 1;
}

int authenticateUser(const char* username, const char* password) {
    FILE* file = fopen("users.txt", "r");
    if (file == NULL) {
        perror("Unable to open users file");
        return 0;
    }
    char storedUsername[100];
    char storedPassword[100];
    char storedCard[20];
    int favoriteCount;
    int isAdmin;
    while (fscanf(file, "%s %s %s %d %d", storedUsername, storedPassword, storedCard, &favoriteCount, &isAdmin) != EOF) {
        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
            fclose(file);
            strcpy(currentUser.username, storedUsername);
            strcpy(currentUser.password, storedPassword);
            strcpy(currentUser.card, storedCard);
            currentUser.favoriteCount = favoriteCount;
            currentUser.isAdmin = isAdmin;
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void loginMenu() {
    char username[100];
    char password[100];
    char card[20];
    int choice;

    while (1) {
        system("cls");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);
            while (1) {
                printf("Enter card number (16 digits): ");
                scanf("%s", card);
                if (validateCard(card)) {
                    break;
                } else {
                    printf("Invalid card number. Please try again.\n");
                }
            }
            if (registerUser(username, password, card)) {
                printf("Registration successful!\n");
            } else {
                printf("Registration failed!\n");
            }
        } else if (choice == 2) {
            printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);
            if (authenticateUser(username, password)) {
                printf("Login successful!\n");
                break;
            } else {
                printf("Invalid username or password!\n");
            }
        } else if (choice == 3) {
            exit(0);
        } else {
            printf("Invalid choice!\n");
        }
        printf("Press any key to continue...");
        _getch();
    }
}

void loadFavorites(Movie** head) {
    char filename[120];
    snprintf(filename, sizeof(filename), "favorites_%s.txt", currentUser.username);
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }

    char title[100];
    int year;
    char country[100];
    char genre[100];
    float rating;
    char buffer[100];

    while (fgets(title, sizeof(title), file)) {
        title[strcspn(title, "\n")] = '\0';
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "%d", &year);
        fgets(country, sizeof(country), file);
        country[strcspn(country, "\n")] = '\0';
        fgets(genre, sizeof(genre), file);
        genre[strcspn(genre, "\n")] = '\0';
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "%f", &rating);

        Movie* newMovie = createMovie(title, year, country, genre, rating);
        addMovie(head, newMovie);
    }

    fclose(file);
}

void addNewMovie(Movie** head) {
    char title[100];
    int year;
    char country[100];
    char genre[100];
    float rating;

    printf("Enter movie title: ");
    scanf(" %[^\n]", title);
    printf("Enter movie year: ");
    scanf("%d", &year);
    printf("Enter movie country: ");
    scanf(" %[^\n]", country);
    printf("Enter movie genre: ");
    scanf(" %[^\n]", genre);
    printf("Enter movie rating: ");
    scanf("%f", &rating);

    Movie* newMovie = createMovie(title, year, country, genre, rating);
    addMovie(head, newMovie);
    saveMoviesToFile(*head);
}

void mainMenu(Movie** head) {
    int choice;

    while (1) {
        system("cls");
        printf("1. View all movies\n");
        printf("2. View favorite movies\n");
        if (currentUser.isAdmin) {
            printf("3. Add new movie\n");
        }
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            displayMovies(*head, 0, "Фильмы");
        } else if (choice == 2) {
            Movie* favorites = NULL;
            loadFavorites(&favorites);
            displayMovies(favorites, 1, "Избранное");
            freeMovies(favorites);
        } else if (choice == 3 && currentUser.isAdmin) {
            addNewMovie(head);
        } else if (choice == 4 || (choice == 3 && !currentUser.isAdmin)) {
            break;
        } else {
            printf("Invalid choice!\n");
        }
        printf("Press any key to continue...");
        _getch();
    }
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    loginMenu();

    FILE* file = fopen("films.txt", "r");
    if (file == NULL) {
        perror("Unable to open file");
        return 1;
    }

    Movie* head = NULL;
    char title[100];
    int year;
    char country[100];
    char genre[100];
    float rating;
    char buffer[100];

    while (fgets(title, sizeof(title), file)) {
        title[strcspn(title, "\n")] = '\0';
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "%d", &year);
        fgets(country, sizeof(country), file);
        country[strcspn(country, "\n")] = '\0';
        fgets(genre, sizeof(genre), file);
        genre[strcspn(genre, "\n")] = '\0';
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "%f", &rating);

        Movie* newMovie = createMovie(title, year, country, genre, rating);
        addMovie(&head, newMovie);
    }

    fclose(file);

    mainMenu(&head);
    freeMovies(head);

    return 0;
}
