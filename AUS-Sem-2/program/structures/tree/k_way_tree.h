#pragma once

#include "tree.h"
#include "../array/array.h"

namespace structures
{
	/// <summary> Vrchol K-cestneho stromu. </summary>
	/// <typeparam name = "T"> Typ dat ukladanych vo vrchole stromu. </typeparam>
	/// <typeparam name = "K"> Kardinalita vrchola. </typeparam>
	template <typename T, int K>
	class KWayTreeNode : public TreeNode<T>
	{
	public:
		/// <summary> Konstruktor. </summary>
		/// <param name = "data"> Data, ktore uchovava. </param>
		KWayTreeNode(T data);

		/// <summary> Kopirovaci konstruktor. </summary>
		/// <param name = "other"> Vrchol k-cestneho stromu, z ktoreho sa prevezmu vlastnosti. </param>
		/// <remarks> Vytvori sa ako plytka kopia vrcholu other. </remarks>
		KWayTreeNode(const KWayTreeNode<T, K>& other);
    
		/// <summary> Destruktor. </summary>
		~KWayTreeNode();

		/// <summary> Vytvori plytku kopiu vrchola (teda skopiruje iba data a smerniky na synov). </summary>
		/// <returns> Vystvorena plytka kopia vrcholu. </returns>
		TreeNode<T>* shallowCopy() override;

		/// <summary> Test, ci je list. </summary>
		/// <returns> true, ak je list, false inak. </returns>
		bool isLeaf() override;

		/// <summary> Spristupni syna vrcholu podla jeho pozicie v tomto vrchole. </summary>
		/// <param name = "order"> Poradie syna. </param>
		/// <exception cref="std::out_of_range"> Vyhodena, ak neexistuje syn s takym poradim. </exception>
		/// <returns> Syn vrcholu. </returns>
		TreeNode<T>* getSon(int order) const override;

		/// <summary> Vlozi noveho syna vrcholu na dane miesto. </summary>
		/// <param name = "order"> Poradie syna. </param>
		/// <exception cref="std::out_of_range"> Vyhodena, ak neexistuje syn s takym poradim. </exception>
		/// <remarks> Pouziva metodu replaceSon. </remarks>
		void insertSon(TreeNode<T>* son, int order) override;

		/// <summary> Nahradi syna vrcholu na danom mieste. </summary>
		/// <param name = "order"> Poradie syna. </param>
		/// <exception cref="std::out_of_range"> Vyhodena, ak je poradie neplatne. </exception>
		/// <returns> Nahradeny syn vrcholu. </returns>
		/// <remarks> Vsetky smerniky zucastnenych vrcholov budu spravne nastavene. </remarks>
		TreeNode<T>* replaceSon(TreeNode<T>* son, int order) override;

		/// <summary> Odstrani syna na danom mieste. </summary>
		/// <param name = "order"> Poradie syna. </param>
		/// <exception cref="std::out_of_range"> Vyhodena, ak neexistuje syn s takym poradim. </exception>
		/// <returns> Odstraneny syn vrcholu. </returns>
		/// <remarks> Pouziva metodu replaceSon s parametrom nullptr. </remarks>
		TreeNode<T>* removeSon(int order) override;

		/// <summary> Vrati stupen vrcholu. </summary>
		/// <returns> Vzdy K. </returns>
		int degree() override;

		/// <summary> Vrati pocet neNULLovych synov. </summary>
		/// <returns> Pocet neNULLovych synov. </returns>
		int numberOfSons();
	protected:
		/// <summary> Synova vrchola. </summary>
		Array<KWayTreeNode<T, K>*>* children_;
	};

	/// <summary> K-cestny strom. </summary>
	/// <typeparam name = "T"> Typ dat ukladanych v strome. </typeparam>
	/// <typeparam name = "K"> Kardinalita stromu. </typeparam>
	template <typename T, int K> 
	class KWayTree : public Tree<T>
	{
	public:
		/// <summary> Konstruktor. </summary>
		KWayTree();

		/// <summary> Kopirovaci konstruktor. </summary>
		/// <param name = "other"> K-cestny strom, z ktoreho sa prevezmu vlastnosti. </param>
		KWayTree(const KWayTree<T, K>& other);
  
		/// <summary> Operacia klonovania. Vytvori a vrati duplikat udajovej struktury. </summary>
		/// <returns> Ukazovatel na klon struktury. </returns>
		Structure* clone() const override;
    
		/// <summary> Vytvori a vrati instanciu vrcholu k-cestneho stromu. </summary>
		/// <returns> Vytvorena instancia vrcholu k-cestneho stromu. </returns>
		TreeNode<T>* createTreeNodeInstance() override;
	};


	template<typename T, int K>
	inline KWayTreeNode<T, K>::KWayTreeNode(T data):
		TreeNode<T>(data),
		children_(new Array<KWayTreeNode<T, K>*>(K))
	{
	}

	template<typename T, int K>
	inline KWayTreeNode<T, K>::KWayTreeNode(const KWayTreeNode<T, K>& other):
		TreeNode<T>(other),
		children_(dynamic_cast<Array<KWayTreeNode<T, K>*>*>(other.children_->clone()))
	{
	}

	template<typename T, int K>
	inline KWayTreeNode<T, K>::~KWayTreeNode()
	{
		// Vymaz synov
		for (int i = 0; i < K; i++)
		{
			delete children_->operator[](i);
		}

		// Vymaz svoje
		delete children_;
		children_ = nullptr;
	}

	template<typename T, int K>
	inline TreeNode<T>* KWayTreeNode<T, K>::shallowCopy()
	{
		return new KWayTreeNode<T, K>(*this);
	}

	template<typename T, int K>
	inline bool KWayTreeNode<T, K>::isLeaf()
	{
		// Prejdi synov, zisti, ci existuje validna hodnota
		for (int i = 0; i < K; i++)
		{
			if (children_->operator[](i))
			{
				// Existuje syn, ktory nieje nullptr
				return false;
			}
		}

		// Kazdy syn je nullptr
		return true;
	}

	template<typename T, int K>
	inline TreeNode<T>* KWayTreeNode<T, K>::getSon(int order) const
	{
		return children_->operator[](order);
	}

	template<typename T, int K>
	inline void KWayTreeNode<T, K>::insertSon(TreeNode<T>* son, int order)
	{
		// Vraj tu nema byt delete na vrateneho, volajuci si to ma zariadit
		replaceSon(son, order);
	}

	template<typename T, int K>
	inline TreeNode<T>* KWayTreeNode<T, K>::replaceSon(TreeNode<T>* son, int order)
	{
		// Nahrad existujuceho za noveho, upravte atributy parent v nich, pozor mozu byt null, pretypuj parameter son

		// Zapamataj si syna
		TreeNode<T>* originalSon = children_->operator[](order);

		// Nahrad syna novym, lepsim synom
		children_->operator[](order) = dynamic_cast<KWayTreeNode<T, K>*>(son);

		if (son)
		{
			son->setParent(this);
		}

		if (originalSon)
		{
			originalSon->resetParent();
		}

		return originalSon;
	}

	template<typename T, int K>
	inline TreeNode<T>* KWayTreeNode<T, K>::removeSon(int order)
	{
		return replaceSon(nullptr, order);
	}

	template<typename T, int K>
	inline int KWayTreeNode<T, K>::degree()
	{
		// K je pocet synov v jednotlivych vrcholoch
		return K;
	}

	template<typename T, int K>
	inline int KWayTreeNode<T, K>::numberOfSons()
	{
		int result = 0;

		// Prejdi synov, napocitaj iba validne hodnoty
		for (int i = 0; i < K; i++)
		{
			if (children_->operator[](i))
			{
				result++;
			}
		}

		return result;
	}

	template<typename T, int K>
	inline KWayTree<T, K>::KWayTree():
		Tree<T>()
	{
	}

	template<typename T, int K>
	inline KWayTree<T, K>::KWayTree(const KWayTree<T, K>& other):
		Tree<T>(other)
	{
	}

	template<typename T, int K>
	inline Structure * KWayTree<T, K>::clone() const
	{
		return new KWayTree<T, K>(*this);
	}

	template<typename T, int K>
	inline TreeNode<T>* KWayTree<T, K>::createTreeNodeInstance()
	{
		T data = T();
		return new KWayTreeNode<T, K>(data);
	}

}

