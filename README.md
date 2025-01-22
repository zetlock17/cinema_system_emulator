# Список задач по проекту (не реализованное из тз)

## Регистрация и авторизация
- Добавить проверку логина:
  - Логин должен быть от 3 до 20 символов.
  - Логин может содержать только латиницу и цифры.
- Добавить проверку пароля:
  - Пароль должен быть от 6 до 20 символов.
  - Пароль должен содержать минимум одну цифру.
  - Пароль должен содержать символы в верхнем и нижнем регистре.
  
## Валидация данных
- Добавить проверку названия фильма:
  - Название фильма должно быть от 1 до 40 символов.
- Добавить проверку названия страны:
  - Название страны должно быть от 1 до 20 символов.
  - Название страны должно начинаться с заглавной буквы.

## Личный кабинет
- Реализовать личный кабинет:
  - Возможность изменения данных пользователя (логин, пароль, email и т.д.).

## Администрирование
- Реализовать удаление фильма из админки:
  - При удалении фильма он должен автоматически удаляться из списка избранного у всех пользователей.
 

<br>
<br>
<br>

# Документация проекта: Эмулятор системы кинотеатра

## Оглавление
1. [Введение](#введение)
2. [Структура проекта](#структура-проекта)
   - [Структура данных](#структура-данных)
   - [Функции](#функции)
3. [Описание работы программы](#описание-работы-программы)
   - [Регистрация и авторизация](#регистрация-и-авторизация)
   - [Просмотр фильмов](#просмотр-фильмов)
   - [Управление избранным](#управление-избранным)
   - [Административные функции](#административные-функции)
4. [Файловая система](#файловая-система)
5. [Заключение](#заключение)

---

## Введение

Данный проект представляет собой эмулятор системы кинотеатра, написанный на языке C. Программа позволяет пользователям регистрироваться, авторизовываться, просматривать список фильмов, добавлять фильмы в избранное, а также управлять списком фильмов (для администраторов). Программа использует текстовые файлы для хранения данных о пользователях, фильмах и избранных фильмах.

---

## Структура проекта

### Структура данных

1. **Movie**  
   Структура, представляющая фильм:
   - `title` — название фильма.
   - `year` — год выпуска.
   - `country` — страна производства.
   - `genre` — жанр фильма.
   - `rating` — рейтинг фильма.
   - `next` и `prev` — указатели на следующий и предыдущий фильмы в списке.

2. **User**  
   Структура, представляющая пользователя:
   - `username` — имя пользователя.
   - `password` — пароль.
   - `card` — номер карты (16 цифр).
   - `favoriteCount` — количество фильмов в избранном.
   - `isAdmin` — флаг, указывающий, является ли пользователь администратором.

3. **currentUser**  
   Глобальная переменная, хранящая данные текущего пользователя.

---

### Функции

1. **Работа с фильмами:**
   - `createMovie` — создает новый объект фильма.
   - `addMovie` — добавляет фильм в список.
   - `removeMovie` — удаляет фильм из списка.
   - `saveMoviesToFile` — сохраняет список фильмов в файл.
   - `displayMovies` — отображает список фильмов.
   - `freeMovies` — освобождает память, выделенную под список фильмов.

2. **Работа с пользователями:**
   - `registerUser` — регистрирует нового пользователя.
   - `authenticateUser` — авторизует пользователя.
   - `updateUserFile` — обновляет данные пользователя в файле.

3. **Работа с избранным:**
   - `isFavorite` — проверяет, находится ли фильм в избранном.
   - `saveFavoriteMovie` — добавляет фильм в избранное.
   - `removeFavoriteMovie` — удаляет фильм из избранного.
   - `loadFavorites` — загружает избранные фильмы пользователя.

4. **Вспомогательные функции:**
   - `validateCard` — проверяет корректность номера карты.
   - `stringCreator` — форматирует строку для вывода.

5. **Основные меню:**
   - `loginMenu` — меню регистрации и авторизации.
   - `mainMenu` — главное меню программы.

---

## Описание работы программы

### Регистрация и авторизация

1. **Регистрация:**
   - Пользователь вводит имя, пароль и номер карты.
   - Программа проверяет корректность номера карты (16 цифр).
   - Данные пользователя сохраняются в файл `users.txt`.

2. **Авторизация:**
   - Пользователь вводит имя и пароль.
   - Программа проверяет данные в файле `users.txt`.
   - Если данные верны, пользователь авторизуется.

---

### Просмотр фильмов

1. **Список фильмов:**
   - Программа загружает фильмы из файла `films.txt`.
   - Пользователь может просматривать фильмы, переключаясь между ними с помощью клавиш `a` (предыдущий) и `d` (следующий).

2. **Информация о фильме:**
   - Для каждого фильма отображается:
     - Название.
     - Год выпуска.
     - Страна производства.
     - Жанр.
     - Рейтинг.
     - Наличие в избранном.

---

### Управление избранным

1. **Добавление в избранное:**
   - Пользователь может добавить фильм в избранное, нажав клавишу `f`.
   - Фильм сохраняется в файл `favorites_<username>.txt`.

2. **Удаление из избранного:**
   - Пользователь может удалить фильм из избранного, нажав клавишу `r`.
   - Фильм удаляется из файла `favorites_<username>.txt`.

---

### Административные функции

1. **Добавление фильма:**
   - Администратор может добавить новый фильм, указав его название, год выпуска, страну, жанр и рейтинг.
   - Фильм сохраняется в файл `films.txt`.

2. **Удаление фильма:**
   - Администратор может удалить фильм, нажав клавишу `g`.

---

## Файловая система

1. **films.txt**  
   Содержит список всех фильмов. Каждый фильм представлен в формате:
   ```
   Название
   Год
   Страна
   Жанр
   Рейтинг
   ```

2. **users.txt**  
   Содержит данные о пользователях. Каждый пользователь представлен в формате:
   ```
   Имя Пароль НомерКарты КоличествоИзбранных Админ
   ```

3. **favorites_<username>.txt**  
   Содержит список избранных фильмов для каждого пользователя. Формат аналогичен `films.txt`.

---

## Заключение

Данный проект представляет собой простую, но функциональную систему управления кинотеатром. Программа позволяет пользователям удобно просматривать фильмы, добавлять их в избранное, а администраторам — управлять списком фильмов. Проект может быть расширен за счет добавления новых функций, таких как покупка билетов, управление сеансами и т.д.
