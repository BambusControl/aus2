#pragma once

#include "table.h"
#include "linked_table.h"
#include "../array/array.h"
#include "../structure_iterator.h"
#include <stdexcept>

namespace structures
{

	/// <summary> Hashovacia tabulka. </summary>
	/// <typeparam name = "K"> Kluc prvkov v tabulke. </typeparam>
	/// <typeparam name = "T"> Typ dat ukladanych v tabulke. </typeparam>
	template <typename K, typename T>
	class HashTable : public Table<K, T>
	{
	private:
		static const int DATA_ARRAY_SIZE = 100;
	public:
		/// <summary> Konstruktor. </summary>
		HashTable();

		/// <summary> Kopirovaci konstruktor. </summary>
		/// <param name = "other"> HashTable, z ktorej sa prevezmu vlastnosti. </param>
		HashTable(const HashTable<K, T>& other);

		/// <summary> Destruktor. </summary>
		~HashTable();

		/// <summary> Operacia klonovania. Vytvori a vrati duplikat udajovej struktury. </summary>
		/// <returns> Ukazovatel na klon struktury. </returns>
		virtual Structure* clone() const;

		/// <summary> Vrati pocet prvkov v tabulke. </summary>
		/// <returns> Pocet prvkov v tabulke. </returns>
		size_t size() const override;

		/// <summary> Operator priradenia. </summary>
		/// <param name = "other"> Tabulka, z ktorej ma prebrat vlastnosti. </param>
		/// <returns> Adresa, na ktorej sa tato tabulka nachadza po priradeni. </returns>
		Table<K, T>& operator=(const Table<K, T>& other) override;

		/// <summary> Operator priradenia. </summary>
		/// <param name = "other"> Hashovacia tabulka, z ktorej ma prebrat vlastnosti. </param>
		/// <returns> Adresa, na ktorej sa tato tabulka nachadza po priradeni. </returns>
		virtual HashTable<K, T>& operator=(const HashTable<K, T>& other);

		/// <summary> Vrati adresou data s danym klucom. </summary>
		/// <param name = "key"> Kluc dat. </param>
		/// <returns> Adresa dat s danym klucom. </returns>
		/// <exception cref="std::out_of_range"> Vyhodena, ak kluc nepatri do tabulky. </exception>  
		T& operator[](const K key) override;

		/// <summary> Vrati hodnotou data s daynm klucom. </summary>
		/// <param name = "key"> Kluc dat. </param>
		/// <returns> Hodnota dat s danym klucom. </returns>
		/// <exception cref="std::out_of_range"> Vyhodena, ak kluc nepatri do zoznamu. </exception>  
		const T operator[](const K key) const override;

		/// <summary> Vlozi data s danym klucom do tabulky. </summary>
		/// <param name = "key"> Kluc vkladanych dat. </param>
		/// <param name = "data"> Vkladane data. </param>
		/// <exception cref="std::logic_error"> Vyhodena, ak tabulka uz obsahuje data s takymto klucom. </exception>  
		void insert(const K& key, const T& data) override;

		/// <summary> Odstrani z tabulky prvok s danym klucom. </summary>
		/// <param name = "key"> Kluc prvku. </param>
		/// <returns> Odstranene data. </returns>
		/// <exception cref="std::logic_error"> Vyhodena, ak tabulka neobsahuje data s takymto klucom. </exception>  
		T remove(const K& key) override;

		/// <summary> Bezpecne ziska data s danym klucom. </summary>
		/// <param name = "key"> Kluc dat. </param>
		/// <param name = "data"> Najdene data (vystupny parameter). </param>
		/// <returns> true, ak sa podarilo najst a naplnit data s danym klucom, false inak. </returns>
		bool tryFind(const K& key, T& data) override;

		/// <summary> Zisti, ci tabulka obsahuje data s danym klucom. </summary>
		/// <param name = "key"> Kluc dat. </param>
		/// <returns> true, tabulka obsahuje dany kluc, false inak. </returns>
		bool containsKey(const K& key) override;

		/// <summary> Vymaze tabulku. </summary>
		void clear() override;

		/// <summary> Vrati skutocny iterator na zaciatok struktury </summary>
		/// <returns> Iterator na zaciatok struktury. </returns>
		/// <remarks> Zabezpecuje polymorfizmus. </remarks>
		Iterator<TableItem<K, T>*>* getBeginIterator() const override;

		/// <summary> Vrati skutocny iterator na koniec struktury </summary>
		/// <returns> Iterator na koniec struktury. </returns>
		/// <remarks> Zabezpecuje polymorfizmus. </remarks>
		Iterator<TableItem<K, T>*>* getEndIterator() const override;
	protected:
		/// <summary> Pole s datami. </summary>
		Array<Table<K, T>*>* data_;
		/// <summary> Pocet prvkov v binarnom vyhladavacom strome. </summary>
		size_t size_;
	private:
		/// <summary> Hashovacia funkcia zahashuje kluc na indexu v poli dat. </summary>
		/// <param name = "key"> Kluc dat. </param>
		/// <returns> Index do pola dat, ktory vznikol zahashovanim kluca. </returns>
		int hashFunction(const K& key) const;

	private:
		/// <summary> Iterator pre HashTable. </summary>
		class HashTableIterator : public Iterator<TableItem<K, T>*>
		{
		public:
			/// <summary> Konstruktor. </summary>
			/// <param name = "hashTable"> Hashovacia tabulka, cez ktoru iteruje. </param>
			/// <param name = "position"> Pozicia, na ktorej zacina. </param>
			HashTableIterator(const HashTable<K, T>* hashTable, int position);

			/// <summary> Destruktor. </summary>
			~HashTableIterator();

			/// <summary> Operator priradenia. Priradi do seba hodnotu druheho iteratora. </summary>
			/// <param name = "other"> Druhy iterator. </param>
			/// <returns> Vrati seba po priradeni. </returns>
			Iterator<TableItem<K, T>*>& operator= (const Iterator<TableItem<K, T>*>& other) override;

			/// <summary> Porovna sa s druhym iteratorom na nerovnost. </summary>
			/// <param name = "other"> Druhy iterator. </param>
			/// <returns> True, ak sa iteratory nerovnaju, false inak. </returns>
			bool operator!=(const Iterator<TableItem<K, T>*>& other) override;

			/// <summary> Vrati data, na ktore aktualne ukazuje iterator. </summary>
			/// <returns> Data, na ktore aktualne ukazuje iterator. </returns>
			TableItem<K, T>* const operator*() override;

			/// <summary> Posunie iterator na dalsi prvok v strukture. </summary>
			/// <returns> Iterator na dalsi prvok v strukture. </returns>
			/// <remarks> Zvycajne vrati seba. Ak vrati iny iterator, povodny bude automaticky zruseny. </remarks>
			Iterator<TableItem<K, T>*>& operator++() override;
		private:
			/// <summary> Hashovacia tabulka, cez ktoru iteruje. </summary>
			const HashTable<K, T>* hashTable_;
			/// <summary> Aktualna pozicia v poli v tabulke. </summary>
			int position_;
			/// <summary> Aktualmy iterator preplnovacej tabulky v poli. </summary>
			Iterator<TableItem<K, T>*>* iterCurr_;
			/// <summary> Koncovy iterator preplnovacej tabulky v poli. </summary>
			Iterator<TableItem<K, T>*>* iterLast_;

			/// <summary> 
			/// Zmeni hodnotu v atribute position_ na najblizsiu preplnovaciu tabulku v poli tabuliek. 
			/// Ak sa taka tabulka najde, nastavi iterCurr_ a iterLast_.
			/// </summary>
			void movePositionInArray();
		};
	};

	template<typename K, typename T>
	inline HashTable<K, T>::HashTable():
		Table<K, T>(),
		data_(new Array<Table<K, T>*>(DATA_ARRAY_SIZE)),
		size_(0)
	{
	}

	template<typename K, typename T>
	inline HashTable<K, T>::HashTable(const HashTable<K, T>& other):
		HashTable()
	{
		*this = other;
	}

	template<typename K, typename T>
	inline HashTable<K, T>::~HashTable()
	{
		//TODO 09: HashTable
	}

	template<typename K, typename T>
	inline Structure * HashTable<K, T>::clone() const
	{
		return new HashTable<K, T>(*this);
	}

	template<typename K, typename T>
	inline size_t HashTable<K, T>::size() const
	{
		//TODO 09: HashTable
		throw std::exception("HashTable<K, T>::size: Not implemented yet.");
	}

	template<typename K, typename T>
	inline Table<K, T>& HashTable<K, T>::operator=(const Table<K, T>& other)
	{
		if (this != &other)
		{
			*this = dynamic_cast<const HashTable<K, T>&>(other);
		}
		return *this;
	}

	template<typename K, typename T>
	inline HashTable<K, T>& HashTable<K, T>::operator=(const HashTable<K, T>& other)
	{
		//TODO 09: HashTable
		throw std::exception("HashTable<K, T>::operator=: Not implemented yet.");
	}

	template<typename K, typename T>
	inline T & HashTable<K, T>::operator[](const K key)
	{
		//TODO 09: HashTable
		throw std::exception("HashTable<K, T>::operator[]: Not implemented yet.");
	}

	template<typename K, typename T>
	inline const T HashTable<K, T>::operator[](const K key) const
	{
		//TODO 09: HashTable
		throw std::exception("HashTable<K, T>::operator[]: Not implemented yet.");
	}

	template<typename K, typename T>
	inline void HashTable<K, T>::insert(const K & key, const T & data)
	{
		//TODO 09: HashTable
		throw std::exception("HashTable<K, T>::insert: Not implemented yet.");
	}

	template<typename K, typename T>
	inline T HashTable<K, T>::remove(const K & key)
	{
		//TODO 09: HashTable
		throw std::exception("HashTable<K, T>::remove: Not implemented yet.");
	}

	template<typename K, typename T>
	inline bool HashTable<K, T>::tryFind(const K & key, T & data)
	{
		//TODO 09: HashTable
		throw std::exception("HashTable<K, T>::tryFind: Not implemented yet.");
	}

	template<typename K, typename T>
	inline bool HashTable<K, T>::containsKey(const K & key)
	{
		//TODO 09: HashTable
		throw std::exception("HashTable<K, T>::containsKey: Not implemented yet.");
	}

	template<typename K, typename T>
	inline void HashTable<K, T>::clear()
	{
		//TODO 09: HashTable
		throw std::exception("HashTable<K, T>::clear: Not implemented yet.");
	}

	template<typename K, typename T>
	inline Iterator<TableItem<K, T>*>* HashTable<K, T>::getBeginIterator() const
	{
		return new HashTableIterator(this, 0);
	}

	template<typename K, typename T>
	inline Iterator<TableItem<K, T>*>* HashTable<K, T>::getEndIterator() const
	{
		return new HashTableIterator(this, static_cast<int>(DATA_ARRAY_SIZE));
	}

	template<typename K, typename T>
	inline int HashTable<K, T>::hashFunction(const K & key) const
	{
		// Knuth Variant on Division: h(k) = k(k + 3) mod m.
		return (key * (key + 3)) % DATA_ARRAY_SIZE;
	}

	template<typename K, typename T>
	inline HashTable<K, T>::HashTableIterator::HashTableIterator(const HashTable<K, T>* hashTable, int position):
		Iterator<TableItem<K, T>*>(),
		hashTable_(hashTable),
		position_(position),
		iterCurr_(nullptr),
		iterLast_(nullptr)
	{
		movePositionInArray();
	}

	template<typename K, typename T>
	inline HashTable<K, T>::HashTableIterator::~HashTableIterator()
	{
		//TODO 09: HashTable<K, T>::HashTableIterator
	}

	template<typename K, typename T>
	inline Iterator<TableItem<K, T>*>& HashTable<K, T>::HashTableIterator::operator=(const Iterator<TableItem<K, T>*>& other)
	{
		//TODO 09: HashTable<K, T>::HashTableIterator
		throw std::exception("HashTable<K, T>::HashTableIterator::operator=: Not implemented yet.");
	}

	template<typename K, typename T>
	inline bool HashTable<K, T>::HashTableIterator::operator!=(const Iterator<TableItem<K, T>*>& other)
	{
		//TODO 09: HashTable<K, T>::HashTableIterator
		throw std::exception("HashTable<K, T>::HashTableIterator::operator!=: Not implemented yet.");
	}

	template<typename K, typename T>
	inline TableItem<K, T>* const HashTable<K, T>::HashTableIterator::operator*()
	{
		//TODO 09: HashTable<K, T>::HashTableIterator
		throw std::exception("HashTable<K, T>::HashTableIterator::operator*: Not implemented yet.");
	}

	template<typename K, typename T>
	inline Iterator<TableItem<K, T>*>& HashTable<K, T>::HashTableIterator::operator++()
	{
		//TODO 09: HashTable<K, T>::HashTableIterator
		throw std::exception("HashTable<K, T>::HashTableIterator::operator++: Not implemented yet.");
	}

	template<typename K, typename T>
	inline void HashTable<K, T>::HashTableIterator::movePositionInArray()
	{
		//TODO 09: HashTable<K, T>::HashTableIterator
		throw std::exception("HashTable<K, T>::HashTableIterator::movePositionInArray: Not implemented yet.");
	}

}