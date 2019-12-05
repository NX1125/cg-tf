/* 
 * File:   stopwatch.h
 * Author: gg
 *
 * Created on October 7, 2019, 11:33 AM
 */

#ifndef STOPWATCH_H
#define STOPWATCH_H

class stopwatch_t {
private:
    /**
     * When the stopwatch started measuring the time from.
     */
    int offset;

    /**
     * The point that were marked by the stopwatch.
     */
    int marked;

public:

    /**
     * Creates a new stopwatch with the current time being the initial offset.
     */
    stopwatch_t() {
        reset();
    }

    ~stopwatch_t();

    /**
     * Restarts this stopwatch at the current time.
     */
    void reset() {
        offset = currentTimeMillis();
        marked = offset;
    }

    /**
     * Mark the current time in the stopwatch.
     */
    void mark() {
        marked = currentTimeMillis();
    }

    /**
     * Returns the number of milliseconds between the reset and mark calls.
     * 
     * @return A value in milliseconds.
     */
    int getMarkedElaspedTimeMillis() {
        return marked - offset;
    }
    
    void setMarkAsOffset() {
        offset = marked;
    }

public:

    /**
     * Returns the number of milliseconds since glutInit was called.
     * 
     * @return A value in milliseconds.
     */
    static int currentTimeMillis();
};

#endif /* STOPWATCH_H */

