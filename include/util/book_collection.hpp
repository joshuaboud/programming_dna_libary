#pragma once

#include <fundamental/book.hpp>
#include <interactive/book_filter.hpp>

#include <algorithm>
#include <array>
#include <span>
#include <tuple>
#include <vector>

class IBookCollection {
protected:
  IBookCollection() = default;

public:
  virtual std::vector<Book> fetchBooks(std::span<BookFilter> filters) = 0;
  virtual bool addNewBook(const Book &book) = 0;
  virtual bool removeBook(const Book &book) = 0;
  virtual bool editBook(const Book &book) = 0;
};

class ListBookCollection : public IBookCollection {

public:
  ListBookCollection(std::span<Book> books)
      : mBooks(books.begin(), books.end()) {}
  ListBookCollection() : ListBookCollection(sDefaultBooks) {}

  std::vector<Book> fetchBooks(std::span<BookFilter> filters) override {
    if (filters.empty())
      return mBooks;
    std::vector<Book> matchingBooks;

    auto predicate = [&filters](const Book &book) {
      for (BookFilter &filter : filters) {
        std::string filterValue = filter.getValue();
        std::string bookValue;
        switch (filter.getKey()) {
        case BookFilter::Key::AUTHOR:
          bookValue = book.getAuthor();
          break;
        case BookFilter::Key::GENRE:
          bookValue = book.getGenre();
          break;
        case BookFilter::Key::ISBN:
          bookValue = book.getIsbn();
          break;
        case BookFilter::Key::TITLE:
          bookValue = book.getTitle();
          break;
        default:
          throw std::logic_error("Unhandled BookFilter::Key in switch statement"
          );
        }
        // to uppercase before testing
        std::transform(
            filterValue.begin(), filterValue.end(), filterValue.begin(),
            [](unsigned char c) { return std::toupper(c); }
        );
        std::transform(
            bookValue.begin(), bookValue.end(), bookValue.begin(),
            [](unsigned char c) { return std::toupper(c); }
        );
        // check for substring match
        if (bookValue.find(filterValue) == std::string::npos) {
          return false;
        } // else continue
      }
      return true;
    };

    std::copy_if(
        mBooks.begin(), mBooks.end(), std::back_inserter(matchingBooks),
        predicate
    );

    return matchingBooks;
  }

  bool addNewBook(const Book &book) {
    if (bookInCollection(book))
      return false;
    mBooks.emplace_back(book);
    return true;
  }

  bool removeBook(const Book &book) {
    if (!bookInCollection(book))
      return false;
    mBooks.erase(
        std::remove_if(
            mBooks.begin(), mBooks.end(),
            [&](const Book &b) { return b.getId() == book.getId(); }
        ),
        mBooks.end()
    );
    return true;
  }

  bool editBook(const Book &book) {
    if (!removeBook(book))
      return false;
    return addNewBook(book);
  }

private:
  std::vector<Book> mBooks;

  bool bookInCollection(const Book &book) {
    return std::find_if(mBooks.begin(), mBooks.end(), [&, this](const Book &b) {
             return b.getId() == book.getId();
           }) != mBooks.end();
  }

  inline static std::array sDefaultBooks = {
      Book("1", "1984", "George Orwell", "Shelf 1", "9780452284234", "Fiction"),
      Book(
          "2", "Harry Potter and the Philosopher's Stone", "J.K. Rowling",
          "Shelf 2", "0747532745", "Fiction"
      ),
      Book(
          "3", "Hitchhiker's Guide to the Galaxy, The", "Douglas Adams",
          "Shelf 42", "0345391802", "Fiction"
      ),
      Book(
          "4",
          "Opening Spaces: An Anthology of Contemporary African Women's "
          "Writing",
          "Yvonne Vera", "Shelf 1", "9780435910105",
          "General & Miscellaneous Literature Anthologies, Anthologies"
      ),
      Book(
          "5",
          "The Daily Show with Jon Stewart Presents Earth (the Book): A "
          "Visitor's Guide to the Human Race",
          "Jon Stewart", "Shelf 1", "9780446579223", "Humor"
      ),
      Book(
          "6", "The Best American Nonrequired Reading 2010", "Dave Eggers",
          "Shelf 2", "9780547241630",
          "American Literature Anthologies, Fiction Subjects"
      ),
      Book(
          "7", "100 Best-Loved Poems", "Philip Smith", "Shelf 4",
          "9780486285535", "Poetry Anthologies"
      ),
      Book(
          "8", "The Best American Short Plays 2008-2009", "Barbara Parisi",
          "Shelf 2", "9781557837608",
          "Drama Anthologies, American Drama, American Literature Anthologies"
      ),
      Book(
          "9", "The Gift of Love", "Lori Foster", "Shelf 3", "9780425234280",
          "Short Story Anthologies, Family & Friendship - Fiction, American "
          "Literature Anthologies"
      ),
      Book(
          "10", "American Fantastic Tales Boxed Set", "Peter Straub", "Shelf 3",
          "9781598530599",
          "Fiction, American Literature Anthologies, Fiction Subjects"
      ),
      Book(
          "11",
          "Unsettling America: An Anthology of Contemporary Multicultural "
          "Poetry",
          "Maria Mazziotti Gillan", "Shelf 2", "9780140237788",
          "Poetry Anthologies, American Poetry, American Literature Anthologies"
      ),
      Book(
          "12", "The Four Seasons: Poems", "J. D. McClatchy", "Shelf 4",
          "9780307268341",
          "Poetry Anthologies, American Poetry, English Poetry, Literature "
          "Anthologies - General & Miscellaneous, English & Irish Literature "
          "Anthologies, American Literature Anthologies"
      ),
      Book(
          "13",
          "The American Stage: Writing on Theater from Washington Irving to "
          "Tony Kushner",
          "Lawrence Senelick", "Shelf 3", "9781598530698",
          "U.S. & Canadian Drama - Literary Criticism, United States - Theater "
          "- History & Criticism, American Literature Anthologies"
      ),
      Book(
          "14", "Three Centuries of American Poetry", "Allen Mandelbaum",
          "Shelf 4", "9780553375183", "Poetry, Anthologies (multiple authors)"
      ),
      Book(
          "15", "Down Time: Great Writers on Diving", "Ed Kittrell", "Shelf 1",
          "9780965834445",
          "Nautical & Maritime Fiction, Sports Essays, Submarines - Military "
          "History, Submarines, Oceanography, Scuba & Snorkeling, Shipwrecks & "
          "Underwater Exploration, Natural Terrain - Oceans & Seas, Literary "
          "Styles & Movements - Fiction, American Literature Antho"
      ),
      Book(
          "16",
          "The Lowell Offering: Writings by New England Mill Women (1840-1845)",
          "Benita Eisler", "Shelf 2", "9780393316858",
          "Social Sciences, Women's Studies"
      ),
      Book(
          "17",
          "The Heath Anthology of American Literature: Volume E: Contemporary "
          "Period (1945 to the Present)",
          "Paul Lauter", "Shelf 2", "9780618533015",
          "American Literature Anthologies"
      ),
      Book(
          "18", "Best American Magazine Writing 2004",
          "American Society Of Magazine Editors", "Shelf 3", "9780060749538",
          "Other Media, Journalism, American Literature Anthologies"
      ),
      Book(
          "19", "Word Virus", "William S. Burroughs", "Shelf 1",
          "9780802136947",
          "Fiction, American Literature Anthologies, US & Canadian Literary "
          "Biography"
      ),
      Book(
          "20",
          "American Fantastic Tales: Terror and the Uncanny from the 1940s to "
          "Now",
          "Peter Straub", "Shelf 2", "9781598530483",
          "Fiction, American Literature Anthologies, Fiction Subjects"
      ),
      Book(
          "21", "Literary Savannah", "Patrick Allen", "Shelf 1",
          "9781892514011", "Travel, Museums, Tours, Points of Interest"
      ),
      Book(
          "22", "New Oxford Book of American Verse, Vol. 1", "Richard Ellmann",
          "Shelf 4", "9780195020588",
          "Poetry Anthologies, American Poetry, American Literature Anthologies"
      ),
      Book(
          "23", "Caroling Dusk", "Countee Cullen", "Shelf 3", "9780806513492",
          "Poetry, American Literature Anthologies, Anthologies"
      ),
      Book(
          "24", "Literary Fort Worth", "Judy Alter", "Shelf 1", "9780875652535",
          "Regional American Anthologies"
      ),
      Book(
          "25", "A River Through Illinois", "Daniel V. Overturf", "Shelf 4",
          "9780809327638",
          "Nature, Marine Terrain, American Literature Anthologies, Travel - "
          "General & Miscellaneous, Aquatic Life & Sciences, U.S. Travel - "
          "States, Photography - Travel, Travel Photography, Natural Terrain"
      ),
      Book(
          "26", "Don't Squat With Yer Spurs On! II, Vol. 2", "Texas Bix Bender",
          "Shelf 2", "9780879058326",
          "American Humor - Peoples & Cultures, American Literature Anthologies"
      ),
      Book(
          "27", "Impossible Princess", "Kevin Killian", "Shelf 4",
          "9780872865280",
          "Fiction, American Literature Anthologies, Anthologies, Gay & "
          "Lesbian Studies, Fiction Subjects"
      ),
      Book(
          "28", "This Bridge We Call Home: Radical Visions for Transformation",
          "Gloria E. Anzaldua", "Shelf 1", "9780415936828",
          "American Literature Anthologies, Women's Biography, Anthologies, "
          "General & Miscellaneous Literary Criticism, Gay & Lesbian Studies, "
          "Women's Biography, Gay & Lesbian Biographies"
      ),
      Book(
          "29", "Appalachia inside Out, Vol. 2", "Robert J. Higgs", "Shelf 1",
          "9780870498763",
          "United States - Civilization, Regional American Anthologies, "
          "Southern Region - History - General & Miscellaneous"
      ),
  };
};

using BookCollection = ListBookCollection;
