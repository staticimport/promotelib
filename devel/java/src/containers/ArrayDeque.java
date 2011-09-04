package com.googlecode.promotelib.containers;

import java.util.AbstractList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;
import java.util.RandomAccess;

import com.googlecode.promotelib.numeric.Numeric;

public class ArrayDeque<E>
extends AbstractList<E>
implements Cloneable, List<E>, RandomAccess
{
  private E[] array_;
  private int mask_;
  private int front_;
  private int back_;
  private int modCount_;

  private static final int DEFAULT_INIT_CAPACITY = 16;

  public ArrayDeque()
  {
    this(null, DEFAULT_INIT_CAPACITY, false);
  }

  public ArrayDeque(final Collection<? extends E> c)
  {
    this(c, Math.max(c.size(), DEFAULT_INIT_CAPACITY), true);
  }

  public ArrayDeque(final int minCapacity)
  {
    this(null, minCapacity, false);
  }

  private ArrayDeque(final Collection<? extends E> c,
                    final int minCapacity,
                    final boolean addCollection)
  {
    array_ = (E[])(new Object[Numeric.powerOf2Above(minCapacity, true)]);
    mask_ = array_.length - 1;
    front_ = back_ = 0;
    modCount_ = 0;
    if( addCollection ) {
      addAll(c);
    }
  }

  @Override
  public boolean add(final E e)
  {
    if( back_ - front_ == array_.length ) {
      setCapacity(array_.length << 1);
    }
    ++modCount_;
    array_[(back_++) & mask_] = e;
    return true;
  }

  @Override
  public void add(final int index, final E e)
  {
    shift(index, 1);
    array_[(front_ + index) & mask_] = e;
  }

  @Override
  public boolean addAll(final Collection<? extends E> c)
  {
    ensureCapacity(size() + c.size());
    if( !c.isEmpty() ) ++modCount_;
    for(final E e : c) {
      array_[(back_++) & mask_] = e;
    }
    return true;
  }

  @Override
  public boolean addAll(final int index, final Collection<? extends E> c)
  {
    shift(index, c.size());
    final Iterator<? extends E> iterator = c.iterator();
    int insertIndex = front_ & index;
    for(final E e : c) {
      array_[(insertIndex++) & mask_] = e;
    }
    return true;
  }

  @Override
  public void clear()
  {
    ++modCount_;
    for(int ii = front_; ii != back_; ++ii) {
      array_[ii & mask_] = null;
    }
    front_ = back_ = 0;
  }
  
  @Override
  public Object clone()
  {
      return new ArrayDeque<E>(this);
  }

  @Override
  public boolean contains(final Object o)
  {
    return indexOf(o) != -1;
  }

  @Override
  public boolean containsAll(final Collection<?> c)
  {
    for(final Object o : c) {
      if( !contains(o) ) {
        return false;
      }
    }
    return true;
  }

  public void ensureCapacity(final int minCapacity)
  {
    if( minCapacity > array_.length ) {
      setCapacity(Numeric.powerOf2Above(minCapacity, true));
    }
  }

  @Override
  public E get(final int index)
  {
    final int frontOffset = front_ + index;
    if( front_ < back_ ) {
      return array_[frontOffset & mask_];
    } else {
      throw new IllegalArgumentException("Index beyond end of list.");
    }
  }

  @Override
  public int indexOf(final Object o)
  {
    if( o != null ) {
      for(int ii = front_; ii != back_; ++ii) {
        final Object entry = array_[ii & mask_]; 
        if( o == entry || o.equals(entry) ) {
          return ii - front_;
        }
      }
    } else {
      for(int ii = front_; ii != back_; ++ii) {
        if( array_[ii & mask_] == null ) {
          return ii - front_;
        }
      }
    }
    return -1;
  }

  @Override
  public boolean isEmpty()
  {
    return front_ == back_;
  }

  @Override
  public Iterator<E> iterator()
  {
    return listIterator();
  }

  @Override
  public int lastIndexOf(final Object o)
  {
    if( o != null ) {
      for(int ii = back_ - 1; ii >= front_; --ii) {
        final Object entry = array_[ii & mask_];
        if( o == entry || o.equals(entry) ) {
          return ii - front_;
        }
      }
    } else {
      for(int ii = back_ - 1; ii >= front_; --ii) {
        if( array_[ii & mask_] == null ) {
          return ii - front_;
        }
      }
    }
    return -1;
  }

  @Override
  public ListIterator<E> listIterator()
  {
    return new ArrayDequeIterator(front_);
  }

  @Override
  public ListIterator<E> listIterator(final int index)
  {
    return new ArrayDequeIterator(front_ + index);
  }

  @Override
  public E remove(final int index)
  {
    final E removed = array_[(front_ + index) & mask_];
    removeRange(index, index + 1);
    return removed;
    /*
    final int realIndex = (front_ + index) & mask_;
    final E removed = array_[realIndex];
    final int pastFront = front_ - realIndex;
    final int beforeBack = realIndex - back_;
    ++modCount_;
    if( pastFront < 0 || (beforeBack > 0 && beforeBack <= pastFront) ) {
      System.arraycopy(array_, realIndex + 1, array_, realIndex, back_ - index - 1);
      array_[(--back_) & mask_] = null;
    } else {
      System.arraycopy(array_, 0, array_, 1, index);
      array_[(front_++) & mask_] = null;
    }
    return removed;*/
  }

  @Override
  public boolean remove(final Object o)
  {
    return (remove(indexOf(o)) != null);
  }

  @Override
  public boolean removeAll(final Collection<?> c)
  {
    return super.removeAll(c);
  }

  public void removeRange(final int fromIndex, final int toIndex)
  {
    final int realFromIndex = (front_ + fromIndex) & mask_;
    final int realLastIndex = (front_ + toIndex - 1) & mask_;
    if( realFromIndex <= realLastIndex ) {
      final int realFrontIndex = front_ & mask_;
      final int offset = toIndex - fromIndex;
      if( realFrontIndex <= realFromIndex ) {
        for(int ii = fromIndex - 1; ii != -1; --ii) {
          array_[ii + offset] = array_[ii];
          array_[ii] = null;
        }
        front_ += offset;
      } else {
        final int realBackIndex = back_ & mask_;
        for(int ii = realLastIndex + 1; ii != realBackIndex; ++ii) {
          array_[ii - offset] = array_[ii];
          array_[ii] = null;
        }
        back_ -= offset;
      }
    } else {
      removeRange(fromIndex, array_.length - realFromIndex);
      removeRange(0, realLastIndex + 1);
    }
  }

  @Override
  public boolean retainAll(final Collection<?> c)
  {
    int newOffset = 0;
    final int size = size();
    for(int ii = 0; ii != size; ++ii) {
      final int realIndex = (front_ + ii) & mask_;
      if( c.contains(c) ) {
        if( newOffset != ii ) {
          array_[(front_ + newOffset) & mask_] = array_[realIndex];
          array_[realIndex] = null;
        }
        ++newOffset;
      } else {
        array_[realIndex] = null;
      }
    }
    back_ = newOffset;
    return true;
  }

  @Override
  public E set(final int index, final E e)
  {
    final int frontOffset = front_ + index;
    if( frontOffset < back_ ) {
      ++modCount_;
      final int realIndex = frontOffset & mask_;
      final E old = array_[realIndex];
      array_[realIndex] = e;
      return old;
    } else {
      throw new IllegalArgumentException("Index past end of list.");
    }
  }

  @Override
  public int size()
  {
    return back_ - front_;
  }

  @Override
  public List<E> subList(final int fromIndex, final int toIndex)
  {
    final int count = toIndex - fromIndex;
    final ArrayDeque<E> list = new ArrayDeque<E>(count);
    final int realFrontIndex = (front_ + fromIndex) & mask_;
    final int realToIndex = (front_ + toIndex - 1) & mask_;
    if( realToIndex >= realFrontIndex ) {
      System.arraycopy(list.array_, 0, array_, realFrontIndex, count);
    } else {
      final int firstCopyCount = array_.length - realFrontIndex;
      System.arraycopy(list.array_, 0, array_, realFrontIndex, firstCopyCount);
      System.arraycopy(list.array_, firstCopyCount, array_, 0, realToIndex);
    }
    list.front_ = 0;
    list.back_ = count;
    return list;
  }

  @Override
  public Object[] toArray()
  {
    return toArray(null);
  }

  @Override
  public <T> T[] toArray(T[] a)
  {
    if( a == null || a.length < array_.length ) {
      a = (T[])(new Object[array_.length]);
    }
    int arrayIndex = 0;
    for(int ii = front_; ii != back_; ++ii) {
      a[arrayIndex++] = (T)array_[ii & mask_];
    }
    return a;
  }
  
  private void setCapacity(final int newCapacity)
  {
    final E[] newArray = (E[])(new Object[newCapacity]);
    final int oldRealFront = front_ & mask_;
    final int oldRealBack = back_ & mask_;
    final int size = back_ - front_;
    if( oldRealFront <= oldRealBack ) {
      System.arraycopy(array_, oldRealFront, newArray, 0, size);
    } else {
      final int beforeWrap = array_.length - oldRealFront;
      System.arraycopy(array_, oldRealFront, newArray, 0, beforeWrap);
      System.arraycopy(array_, 0, newArray, beforeWrap, oldRealBack);
    }
    array_ = newArray;
    mask_ = newCapacity;
    front_ = 0;
    back_ = size;
  }
  
  private void shift(final int index, final int count)
  {
    final int oldSize = size();
    if( oldSize + count <= array_.length ) {
      final int oldOffset = front_ + index;
      final int newOffset = oldOffset + count;
      final int oldRealIndex = oldOffset & mask_;
      if( index > (oldSize >> 1) ) {
        final int newRealIndex = (newOffset + count) & mask_;
        if( oldRealIndex < newRealIndex ) {
          System.arraycopy(array_, oldRealIndex, array_, newRealIndex, oldSize - index);
        } else {
          for(int ii = index; ii != oldSize; ++ii) {
            array_[(newOffset + ii) & mask_] = array_[(oldOffset + ii) & mask_];
          }
        }
        back_ += count;
      } else {
        final int newRealIndex = (newOffset - count) & mask_;
        if( oldRealIndex > newRealIndex ) {
          System.arraycopy(array_, oldRealIndex, array_, newRealIndex, index - 1);
        } else {
          for(int ii = 0; ii != index; ++ii) {
            array_[(newOffset + ii) & mask_] = array_[(oldOffset + ii) & mask_];
          }
        }
        front_ -= count;
      }
    } else {
      final E[] newArray = (E[])(new Object[Numeric.powerOf2Above(oldSize+count,true)]);
      int ii = 0;
      for( ; ii != index; ++ii) {
        newArray[ii] = array_[(front_ + ii) & mask_];
      }
      ii += count;
      for(int jj = oldSize - index; jj != -1; --jj) {
        newArray[ii] = array_[(front_ + jj) & mask_];
      }
      array_ = newArray;
      mask_ = newArray.length - 1;
      front_ = 0;
      back_ = oldSize + count;
    }
  }

  private class ArrayDequeIterator implements ListIterator<E>
  {
    private int expectedModCount_;
    private final int size;
    private int nextIndex_;

    public ArrayDequeIterator(final int start)
    {
      expectedModCount_ = modCount_;
      size = size();
      nextIndex_ = start;
    }

    @Override
    public void add(final E e)
    {
      if( modCount_ != expectedModCount_ ) {
        throw new ConcurrentModificationException("ArrayDeque has changed!");
      } else if( hasNext() ) {
        insert(nextIndex_, e);
      } else {
        add(e);
      }
      expectedModCount_ = modCount_;
    }

    @Override
    public boolean hasNext()
    {
      if( modCount_ != expectedModCount_ ) {
        throw new ConcurrentModificationException("ArrayDeque has changed!");
      }
      return nextIndex_ != size;
    }

    @Override
    public boolean hasPrevious()
    {
      if( modCount_ != expectedModCount_ ) {
        throw new ConcurrentModificationException("ArrayDeque has changed!");
      }
      return nextIndex_ != 0;
    }

    @Override
    public E next()
    {
      if( modCount_ != expectedModCount_ ) {
        throw new ConcurrentModificationException("ArrayDeque has changed!");
      }
      if( hasNext() ) {
        return get(nextIndex_++);
      } else {
        throw new RuntimeException("Iterated past end of ArrayDeque!");
      }
    }

    @Override
    public int nextIndex()
    {
      if( modCount_ != expectedModCount_ ) {
        throw new ConcurrentModificationException("ArrayDeque has changed!");
      }
      if( hasNext() ) {
        return nextIndex_++;
      } else {
        throw new RuntimeException("Iterated past end of ArrayDeque!");
      }
    }

    @Override
    public E previous()
    {
      if( modCount_ != expectedModCount_ ) {
        throw new ConcurrentModificationException("ArrayDeque has changed!");
      }
      if( hasPrevious() ) {
        return get(--nextIndex_);
      } else {
        throw new RuntimeException("Reverse-iterated beyond start of ArrayDeque.");
      }
    }

    @Override
    public int previousIndex()
    {
      if( modCount_ != expectedModCount_ ) {
        throw new ConcurrentModificationException("ArrayDeque has changed!");
      }
      if( hasPrevious() ) {
        return --nextIndex_;
      } else {
        throw new RuntimeException("Reverse-iterated beyond start of ArrayDeque.");
      }
    }

    @Override
    public void remove()
    {
      if( modCount_ != expectedModCount_ ) {
        throw new ConcurrentModificationException("ArrayDeque has changed!");
      }
      remove(--nextIndex_);
      expectedModCount_ = modCount_;
    }

    @Override
    public void set(final E e)
    {
      if( modCount_ != expectedModCount_ ) {
        throw new ConcurrentModificationException("ArrayDeque has changed!");
      }
      set(nextIndex_ - 1, e);
      expectedModCount_ = modCount_;
    }
  }
}

