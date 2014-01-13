/**
 * Copyright 2013 Truphone
 */
#ifndef BUFFER_H_
#define BUFFER_H_

#include <string.h>
#include <QString>

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * @brief Internal class
     *
     * @since test-cascades 1.0.0
     */
    class BufferPrivate;
    /*!
     * \brief The Buffer class provides access to a number
     * of 1KiB buffers declared on the heap. Un-used buffers
     * aren't destroyed but are kept and used when a new buffer
     * is allocated.
     *
     * @since test-cascades 1.0.0
     */
    class Buffer
    {
        public:
            /*!
             * \brief Buffer Create a new buffer
             *
             * @since test-cascades 1.0.0
             */
            Buffer();
            /*!
             * \brief Buffer Create a new buffer with an initial value
             *
             * \param v The initial value
             *
             * @since test-cascades 1.0.1
             */
            explicit Buffer(const char * v);
            /*!
             * \brief ~Buffer Free the buffer (memory will be returned to the pool)
             *
             * @since test-cascades 1.0.0
             */
            virtual ~Buffer();
            /*!
             * \brief data Return a read-write pointer to the buffer
             *
             * \return R/W char* pointer to the buffer
             *
             * @since test-cascades 1.0.0
             */
            char * data() const;
            /*!
             * \brief cdata Return a read-only pointer to the buffer
             *
             * \return  RO char* pointer to the buffer
             *
             * @since test-cascades 1.0.0
             */
            const char * cdata() const;
            /*!
             * \brief length The length of the allocated buffer
             *
             * \return The number of bytes the buffer has - not the size
             * of bytes used (or the string length)
             *
             * @since test-cascades 1.0.0
             */
            size_t length() const;
            /*!
             * \brief strlen The length of the buffer if it is being used
             * as a string
             *
             * \return The size, in bytes, of the string.
             *
             * @since test-cascades 1.0.0
             */
            inline size_t strlen() const
            {
#if defined (BUFFER_ON_HOST)
                return strnlen(this->cdata(), this->length());
#else
                return std::strnlen(this->cdata(), this->length());
#endif /* BUFFER_ON_HOST */
            }
            /*!
             * \brief clean Clears the buffer with zeros
             *
             * @since test-cascades 1.0.0
             */
            inline void clean()
            {
                memset(this->data(), 0, this->length());
            }
            /*!
             * \brief operator QString Create a new QString from the buffer
             *
             * @since test-cascades 1.0.0
             */
            operator QString() const
            {
                return QString(this->cdata());
            }
        protected:
        private:
            /*!
             * \brief pvt Pointer to the private data
             */
            void init();
            BufferPrivate * pvt;
            Buffer(const Buffer&) {throw 0;}
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // BUFFER_H_
