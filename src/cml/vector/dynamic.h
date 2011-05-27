/* -*- C++ -*- ------------------------------------------------------------
 
Copyright (c) 2007 Jesse Anders and Demian Nave http://cmldev.net/

The Configurable Math Library (CML) is distributed under the terms of the
Boost Software License, v1.0 (see cml/LICENSE for details).

 *-----------------------------------------------------------------------*/
/** @file
 *  @brief Specialization for resizeable, dynamic-memory vector.
 */

#ifndef dynamic_vector_h
#define dynamic_vector_h

#include <cml/core/dynamic_1D.h>
#include <cml/vector/vector_expr.h>
#include <cml/vector/class_ops.h>
#include <cml/vector/vector_unroller.h>

namespace cml {

/** Resizeable, dynamic-memory vector. */
template<typename Element, typename Alloc>
class vector< Element, dynamic<Alloc> >
: public dynamic_1D<Element,Alloc>
{
  public:

    /* Shorthand for the generator: */
    typedef dynamic<> storage_type;
    typedef dynamic<Alloc> generator_type;

    /* Shorthand for the array type: */
    typedef dynamic_1D<Element,Alloc> array_type;

    /* Shorthand for the type of this vector: */
    typedef vector<Element,generator_type> vector_type;

    /* The vector coordinate type: */
    typedef Element coordinate_type;

    /* For integration into the expression template code: */
    typedef vector_type expr_type;

    /* For integration into the expression template code: */
    typedef vector_type temporary_type;

    /* The type for a vector in one lower dimension: */
    typedef vector_type subvector_type;

    /* Standard: */
    typedef typename array_type::value_type value_type;
    typedef typename array_type::reference reference;
    typedef typename array_type::const_reference const_reference;

    /* For integration into the expression templates code: */
    typedef vector_type& expr_reference;
    typedef const vector_type& expr_const_reference;

    /* For matching by storage type: */
    typedef typename array_type::memory_tag memory_tag;

    /* For matching by size type: */
    typedef typename array_type::size_tag size_tag;

    /* For matching by resizability: */
    typedef typename array_type::resizing_tag resizing_tag;

    /* For matching by result-type: */
    typedef cml::et::vector_result_tag result_tag;

    /* For matching by assignability: */
    typedef cml::et::assignable_tag assignable_tag;


  public:

    /** Return square of the length. */
    value_type length_squared() const {
        return cml::dot(*this,*this);
    }

    /** Return the length. */
    value_type length() const {
        return std::sqrt(length_squared());
    }

    /** Normalize the vector. */
    vector_type& normalize() {
        return (*this /= length());
    }

    /** Set this vector to [0]. */
    vector_type& zero() {
        typedef cml::et::OpAssign<Element,Element> OpT;
        cml::et::UnrollAssignment<OpT>(*this,Element(0));
        return *this;
    }

		/** Set this vector to [0]. */
    vector_type& Zero() {
        return this->zero();
    }

		/** Return a vector with zeros */
    static vector_type Zero(int size) {
        vector_type result (size);
				result.Zero();
        return result;
		}

		/** Return a vector with zeros */
    static vector_type Constant(int size, const value_type &value) {
		    vector_type result (size);
				int i;
				for (i = 0; i < size; i++)
					result[i] = value;

        return result;
    }

    /** Set this vector to a cardinal vector. */
    vector_type& cardinal(size_t i) {
        zero();
        (*this)[i] = Element(1);
        return *this;
    }

    /** Pairwise minimum of this vector with another. */
    template<typename E, class AT>
    void minimize(const vector<E,AT>& v) {
      /* XXX This should probably use ScalarPromote: */
      for (size_t i = 0; i < this->size(); ++i) {
        (*this)[i] = std::min((*this)[i],v[i]);
      }
    }

    /** Pairwise maximum of this vector with another. */
    template<typename E, class AT>
    void maximize(const vector<E,AT>& v) {
      /* XXX This should probably use ScalarPromote: */
      for (size_t i = 0; i < this->size(); ++i) {
        (*this)[i] = std::max((*this)[i],v[i]);
      }
    }

    /** Fill vector with random elements. */
    void random(value_type min, value_type max) {
        for (size_t i = 0; i < this->size(); ++i) {
            (*this)[i] = cml::random_real(min,max);
        }
    }

    /** Return a subvector by removing element i.
     *
     * @internal This is horribly inefficient...
     */
    subvector_type subvector(size_t i) const {
        subvector_type s; s.resize(this->size()-1);
        for(size_t m = 0, n = 0; m < this->size(); ++ m)
            if(m != i) s[n++] = (*this)[m];
        return s;
    };


  public:

    /** Default constructor. */
    vector() : array_type() {}

    /** Construct given array size. */
    vector(size_t N) : array_type(N) {}

    /** Construct given array size and standard value */
    explicit vector(size_t N, const Element stdval) : array_type(N) {
			for (size_t i = 0; i < N; i++)
				(*this)[i] = stdval;
		}

  public:

    /* Define common class operators: */

   	explicit CML_CONSTRUCT_VEC_2(: array_type())
    explicit CML_CONSTRUCT_VEC_3(: array_type())
    explicit CML_CONSTRUCT_VEC_4(: array_type())

    CML_VEC_COPY_FROM_ARRAY(: array_type())
    CML_VEC_COPY_FROM_VECTYPE(: array_type())
    CML_VEC_COPY_FROM_VEC
    CML_VEC_COPY_FROM_VECXPR

    CML_ASSIGN_VEC_2
    CML_ASSIGN_VEC_3
    CML_ASSIGN_VEC_4

    CML_VEC_ASSIGN_FROM_VECTYPE

    CML_VEC_ASSIGN_FROM_VEC(=, cml::et::OpAssign)
    CML_VEC_ASSIGN_FROM_VEC(+=, cml::et::OpAddAssign)
    CML_VEC_ASSIGN_FROM_VEC(-=, cml::et::OpSubAssign)

    CML_VEC_ASSIGN_FROM_VECXPR(=, cml::et::OpAssign)
    CML_VEC_ASSIGN_FROM_VECXPR(+=, cml::et::OpAddAssign)
    CML_VEC_ASSIGN_FROM_VECXPR(-=, cml::et::OpSubAssign)

    CML_VEC_ASSIGN_FROM_SCALAR(*=, cml::et::OpMulAssign)
    CML_VEC_ASSIGN_FROM_SCALAR(/=, cml::et::OpDivAssign)
};

} // namespace cml

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp
