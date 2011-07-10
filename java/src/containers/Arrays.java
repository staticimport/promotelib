package com.googlecode.promotelib.containers;

import java.lang.reflect.Array;
import java.util.Collection;
import java.util.ListIterator;

public final class Arrays
{
	private Arrays() { }
	
	@SuppressWarnings("unchecked")
	public static final <T> T[] add(final Class<?> clazz,
	                                final T[] oldArray,
	                                final T newItem)
	{
		final T[] newArray = (T[])(Array.newInstance(clazz, oldArray.length + 1));
		System.arraycopy(oldArray, 0, newArray, 0, oldArray.length);
		newArray[oldArray.length] = newItem;
		return newArray;
	}
	
	public static final double[] add(final double[] oldArray,
	                                 final double newItem)
	{
		final double[] newArray = new double[oldArray.length + 1];
		System.arraycopy(oldArray, 0, newArray, 0, oldArray.length);
		newArray[oldArray.length] = newItem;
		return newArray;
	}
	
	@SuppressWarnings("unchecked")
	public static final <T> T[] add(final Class<?> clazz,
	                                final T[] oldArray,
	                                final int index,
	                                final T newItem)
	{
		final T[] newArray = (T[])(Array.newInstance(clazz, oldArray.length + 1));
		System.arraycopy(oldArray, 0, newArray, 0, index);
		newArray[index] = newItem;
		System.arraycopy(oldArray, index, newArray, index + 1, newArray.length - index);
		return newArray;
	}
	
	@SuppressWarnings("unchecked")
	public static final <T> T[] addAll(final Class<?> clazz,
	                                   final T[] oldArray, 
	                                   final T...newItems) 
	{
		final T[] newArray = (T[])(Array.newInstance(clazz, oldArray.length + newItems.length));
		System.arraycopy(oldArray, 0, newArray, 0, oldArray.length);
		for(int ii = 0; ii < newItems.length; ii++)
			newArray[oldArray.length + ii] = newItems[ii];
		return newArray;
	}
	
	@SuppressWarnings("unchecked")
	public static final <T> T[] addAll(final Class<?> clazz,
	                                   final T[] oldArray,
	                                   final Collection<? extends T> newItems)
	{
		final int newItemCount = newItems.size();
		final T[] newArray = (T[])(Array.newInstance(clazz, oldArray.length + newItemCount));
		System.arraycopy(oldArray, 0, newArray, 0, oldArray.length);
		int addIndex = oldArray.length;
		for(final T item : newItems)
			newArray[addIndex++] = item;
		return newArray;
	}
	
	public static final <T> boolean contains(final T[] array,
	                                         final T item)
	{
		final boolean notNull = item != null;
		for(final T t : array) {
			if( t == item || (notNull && item.equals(t)) )
				return true;
		}
		return false;
	}
	
	public static final <T> boolean containsAll(final T[] array,
	                                            final T...items)
	{
		top: for(final T item : items) {
			final boolean notNull = item != null;
			for(final T t : array) {
				if( t == item || (notNull && item.equals(t)) )
					continue top;
			}
			return false;
		}
		return true;
	}
	
	public static final <T> boolean containsAll(final T[] array,
	                                            final Collection<? extends T> items)
	{
		top: for(final T item : items) {
			final boolean notNull = item != null;
			for(final T t : array) {
				if( t == item || (notNull && item.equals(t)) )
					continue top;
			}
			return false;
		}
		return true;
	}
	
	@SuppressWarnings("unchecked")
	public static final <T> T[] createArray(final Class<?> clazz,
	                                        final Object...objs)
	{
		final T[] newArray = (T[])(Array.newInstance(clazz, objs.length));
		for(int ii = 0; ii < objs.length; ii++)
			newArray[ii] = (T)objs[ii];
		return newArray;
	}
	
	@SuppressWarnings("unchecked")
	public static final <T> T[] createEmptyArray(final Class<?> clazz,
	                                             final int length)
	{
		return (T[])(Array.newInstance(clazz, length));
	}
	
	public static final <T> int indexOf(final T[] array,
	                                    final T item)
	{
		final boolean notNull = item != null;
		for(int ii = 0; ii < array.length; ii++) {
			final T t = array[ii];
			if( t == item || (notNull && item.equals(t)) )
				return ii;
		}
		return -1;
	}
	
	public static final <T> ListIterator<T> iterator(final T[] array)
	{
		return new ArrayIterator<T>(array);
	}
	
	public static final <T> ListIterator<T> iterator(final T[] array,
	                                                 final int startIndex)
	{
		return new ArrayIterator<T>(array, startIndex);
	}
	
	public static final <T> ListIterator<T> iterator(final T[] array,
	                                                 final int startIndex,
	                                                 final int endIndex)
	{
		return new ArrayIterator<T>(array, startIndex, endIndex);
	}
	
	public static final <T> int lastIndexOf(final T[] array,
	                                        final T item)
	{
		final boolean notNull = item != null;
		for(int ii = array.length - 1; ii >= 0; ii++) {
			final T t = array[ii];
			if( t == item || (notNull && item.equals(t)) )
				return ii;
		}
		return -1;
	}
	
	public static final double max(final double[] array)
	{
		double max = Double.MIN_VALUE;
		for(final double d : array) {
			if( max < d )
				max = d;
		}
		return max;
	}
	
	public static final int max(final int[] array)
	{
		int max = Integer.MIN_VALUE;
		for(final int i : array) {
			if( max < i )
				max = i;
		}
		return max;
	}
	
	public static final double min(final double[] array)
	{
		double min = Double.MAX_VALUE;
		for(final double d : array) {
			if( d < min )
				min = d;
		}
		return min;
	}
	
	public static final int min(final int[] array)
	{
		int min = Integer.MAX_VALUE;
		for(final int i : array) {
			if( i < min )
				min = i;
		}
		return min;
	}
	
	@SuppressWarnings("unchecked")
	public static final <T> T[] remove(final Class<?> clazz,
	                                   final T[] oldArray,
	                                   final int index)
	{
		final T[] newArray = (T[])(new Object[oldArray.length - 1]);
		System.arraycopy(oldArray, 0, newArray, 0, index);
		System.arraycopy(oldArray, index + 1, newArray, index, oldArray.length - index);
		return newArray;
	}
	
	@SuppressWarnings("unchecked")
	public static final <T> T[] remove(final Class<?> clazz,
	                                   final T[] oldArray,
	                                   final T item)
	{
		final boolean notNull = item != null;
		final T[] newArray = (T[])(Array.newInstance(clazz, oldArray.length - 1));
		int index = 0;
		while( index < oldArray.length ) {
			final T t = oldArray[index];
			if( t == item || (notNull && item.equals(t)) )
				break;
			else {
				newArray[index] = oldArray[index];
				index++;
			}
		}
		
		index++;
		if( index < oldArray.length )
			System.arraycopy(oldArray, index, newArray, index - 1, newArray.length - index);
		return newArray;
	}
	
	@SuppressWarnings("unchecked")
	public static final <T> T[] subArray(final Class<? extends T> clazz,
	                                     final T[] array,
	                                     final int fromIndex,
	                                     final int toIndex)
	{
		final int count = toIndex - fromIndex;
		final T[] subArray = (T[])(Array.newInstance(clazz, count));
		System.arraycopy(array, fromIndex, subArray, 0, count);
		return subArray;
	}
	
	private static final class ArrayIterator<T> implements ListIterator<T> {
	    
	    public ArrayIterator(T[] array) {
	        this(array, 0, array == null ? 0 : array.length); 
	    }
	    public ArrayIterator(T[] array, int startIndex) { 
	        this(array, startIndex, array == null ? 0 : array.length); 
	    }
	    public ArrayIterator(T[] array, int startIndex, int endIndex) {
	        if( array == null )
	            throw new IllegalArgumentException("Array must be non-null.");
	        else if( startIndex < 0 || startIndex >= array.length)
	            throw new IllegalArgumentException("StartIndex must be in 0,...,array.length - 1.");
	        else if( endIndex < 1 || endIndex > array.length )
	            throw new IllegalArgumentException("EndIndex must be in 1,...,array.length.");
	        else if( startIndex >= endIndex )
	            throw new IllegalArgumentException("StartIndex must be less than EndIndex.");    
	        
	        array_ = array;
	        startIndex_ = startIndex;
	        endIndex_ = endIndex;
	        nextIndex_ = startIndex;
	    }
	    
	    @Override
	    public void add(T o) {
	        throw new UnsupportedOperationException("Cannot add elements to array.");
	    }
	    
	    @Override
	    public boolean hasNext() {
	        return nextIndex_ >= startIndex_ && nextIndex_ < endIndex_;
	    }
	    
	    @Override
	    public boolean hasPrevious() {
	        return nextIndex_ >= startIndex_ + 1 && nextIndex_ < endIndex_;
	    }
	    
	    @Override
	    public T next() {
	        return array_[nextIndex_++];
	    }
	    
	    @Override
	    public int nextIndex() {
	        return nextIndex_;
	    }
	    
	    @Override
	    public T previous() {
	        return array_[--nextIndex_];
	    }
	    
	    @Override
	    public int previousIndex() {
	        return nextIndex_ - 1;
	    }
	    
	    @Override
	    public void remove() {
	        throw new UnsupportedOperationException("Cannot remove elements from array.");
	    }
	    
	    @Override
	    public void set(T o) {
	        array_[nextIndex_++] = o;
	    }
	    
	    private final T[] array_;
	    private final int startIndex_, endIndex_;
	    private int nextIndex_;
	}
}
