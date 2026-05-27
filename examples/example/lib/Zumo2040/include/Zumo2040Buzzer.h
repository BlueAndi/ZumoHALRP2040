/* MIT License
 *
 * Copyright (c) 2026 - 2026 Andreas Merkle <web@blue-andi.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*******************************************************************************
    DESCRIPTION
*******************************************************************************/
/**
 * @brief Buzzer driver
 * @author Felix Reitenauer
 *
 * @addtogroup Zumo2040Buzzer
 *
 * @{
 */

#ifndef ZUMO2040BUZZER_H
#define ZUMO2040BUZZER_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Zumo2040ErrorCodes.h"
#include "Zumo2040Pin.h"
#include <cstdint>
#include <pico/time.h>
/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

enum BuzzerStatus
{
    /** Indicates that the buzzer is disabled. */
    BUZZER_DISABLED = false,

    /** Indicates that the buzzer is enabled. */
    BUZZER_ENABLED = true
};

enum BuzzerPlaymode
{
    /** The next note is started automatically after the current note has finished. */
    PLAY_AUTOMATIC,

    /** The next note is started only when playCheck() is called. */
    DONT_PLAY_AUTOMATIC
};

enum StaccatoMode
{
    /** Indicates that staccato is disabled. */
    STACCATO_DISABLED = false,
    /** Indicates that staccato is enabled. */
    STACCATO_ENABLED = true
};

/** Driver for the Zumo2040Buzzer. */
class Zumo2040Buzzer
{
    public:
        /** Initializes the members. */
        Zumo2040Buzzer();

        /** Default destructor. */
        ~Zumo2040Buzzer() = default;

        /** Delete copy constructor. */
        Zumo2040Buzzer(const Zumo2040Buzzer&) = delete;

        /** Delete copy assignment operator. */
        Zumo2040Buzzer& operator=(const Zumo2040Buzzer&) = delete;

        /**
         * @brief Set the play mode.
         *
         * If automatic mode is selected while the buzzer is disabled, the next note
         * of the active sequence is started immediately.
         *
         * @param mode Playback mode to use.
         *
         * @return The current error code.
         */
        ErrorCode playMode(BuzzerPlaymode mode);

        /**
         * @brief Continue playback in non-automatic play mode.
         *
         * If the previous note has finished and a sequence is active, the next note is
         * started.
         *
         * @return The current buzzer status.
         */
        BuzzerStatus playCheck();

        /**
         * @brief Play a frequency for a given duration and volume.
         *
         * If the divide-by-10 flag is set in the frequency value, the remaining value
         * is interpreted in 0.1 Hz units. Otherwise, the value is interpreted in Hz.
         *
         * @param freq Frequency value to play, including the optional divide-by-10 flag.
         * @param dur Duration in ms.
         * @param volume The valid range is 0 to 15, where 15 is the maximum volume. Each lower
         * volume step halves the PWM compare value. Very low compare values may reach
         * zero before all volume steps can be represented.
         *
         * @return The current error code.
         */
        ErrorCode playFrequency(uint16_t freq, uint32_t dur, uint16_t volume);

        /**
         * @brief Play a note sequence.
         *
         * The note sequence is interpreted as a small music command string. In
         * automatic play mode, the complete sequence is advanced automatically. In
         * check mode, the application has to call playCheck() regularly, for example
         * from the main loop, to continue with the next note. The play mode may be
         * changed while a sequence is active.
         *
         * Notes are written as the characters A, B, C, D, E, F and G. By default,
         * notes are played as quarter notes with a duration of 500 ms, which
         * corresponds to a tempo of 120 beats per minute. A different note length can
         * be selected by appending a number directly after the note. For example, C8
         * plays C as an eighth note. The character R inserts a rest.
         *
         * Supported sequence commands:
         * - '>' raises the next note by one octave.
         * - '<' lowers the next note by one octave.
         * - '+' or '#' after a note raises it by one semitone.
         * - '-' after a note lowers it by one semitone.
         * - '.' after a note increases its duration by half of its current value.
         *   Additional dots add half of the previous dot extension.
         * - 'O' followed by a number sets the octave. The default is O4.
         * - 'T' followed by a number sets the tempo in beats per minute. The default
         *   is T120.
         * - 'L' followed by a number sets the default note length, for example L4 for
         *   quarter notes, L8 for eighth notes or L16 for sixteenth notes. The default
         *   is L4.
         * - 'V' followed by a value from 0 to 15 sets the volume. The default is V15.
         * - 'MS' enables staccato playback. Each following note is played for half of
         *   its duration, followed by an equally long pause.
         * - 'ML' enables legato playback. Each following note is played for its full
         *   duration. This is the default.
         * - '!' restores the persistent playback settings to their defaults.
         *
         * Example for a C major scale:
         * @code
         * play("L16 V8 cdefgab>cbagfedc");
         * @endcode
         *
         * Example melody:
         * @code
         * play("T240 L8 a gafaeada c+adaeafa <aa<bac#ada c#adaeaf4");
         * @endcode
         *
         * @param notes Note sequence to play. The string is not copied and must remain
         *              valid for the complete playback duration, e.g. as a string
         *              literal or static const character array.
         *
         * @return The current error code.
         */
        ErrorCode play(const char* notes);

        /**
         * @brief Stop the active playback.
         *
         * This stops the current tone, cancels the active alarm and clears the current
         * note sequence.
         */
        void stopPlaying();

        /**
         * @brief Check whether the buzzer is playing.
         *
         * The buzzer is considered active while a tone is currently played or a note
         * sequence is still pending.
         *
         * @return true if the buzzer is active, otherwise false.
         */
        bool isPlaying() const;

        /**
         * @brief Get the last error code.
         *
         * @return The last error code.
         */
        ErrorCode getLastError() const;

    private:
        /**
         * @brief Timer callback used when the current tone duration has elapsed.
         *
         * The callback disables the PWM output and advances the active sequence in
         * automatic playback mode. A return value of 0 prevents the same alarm from
         * being rescheduled automatically.
         *
         * @param id Alarm ID passed by the Pico SDK.
         * @param user_data Pointer to the buzzer instance.
         *
         * @return Always 0.
         */
        static int64_t timerDone(alarm_id_t id, void* user_data);

        /**
         * @brief Plays the specified note for the specified duration with the specified
         * volume.
         *
         * @param note Note which will be played. Range is from E1 to D#9.
         * The value 255 is used for a silent note.
         * @param dur The duration which the note will be played for in ms.
         * @param volume The valid range is 0 to 15, where 15 is the maximum volume. Each lower
         * volume step halves the PWM compare value. Very low compare values may reach
         * zero before all volume steps can be represented.
         *
         * @return The current error code.
         */
        ErrorCode playNote(uint8_t note, uint32_t dur, uint16_t volume);

        /**
         * @brief Get the current character in the note sequence.
         *
         * @return The current character in the note sequence.
         */
        char currentCharacter();

        /**
         * @brief Read a decimal number from the current sequence position.
         *
         * The sequence pointer is advanced to the first character after the number.
         *
         * @return Parsed number.
         */
        uint32_t getNumber();

        /**
         * @brief Parse and play the next command sequence and note from the active note sequence.
         *
         * @return The current error code.
         */
        ErrorCode nextNote();

        /** Pointer to the active note sequence. */
        const char * volatile m_buzzerSequence;

        /** Indicates the current buzzer status. */
        BuzzerStatus m_buzzerStatus;

        /** Indicates the current alarm status. */
        AlarmStatus m_alarmStatus;

        /** ID of the current alarm. */
        alarm_id_t m_alarmID;

        /** Indicates the current playmode. */
        BuzzerPlaymode m_playmode;

        /** Last occurred error code. */
        ErrorCode m_error;

        /** Current default octave. */
        volatile uint32_t m_octave;

        /** Current whole note duration in ms. */
        volatile uint32_t m_wholeNoteDuration;

        /** Current default note type, e.g. 4 for quarter notes. */
        volatile uint32_t m_noteType;

        /** Current default note duration in ms. */
        volatile uint32_t m_duration;

        /** Current default volume. */
        volatile uint16_t m_volume;

        /** Current staccato mode. */
        volatile StaccatoMode m_staccato;

        /** Current staccato rest duration in ms. */
        volatile uint32_t m_staccatoRestDuration;
};
/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* ZUMO2040BUZZER_H */
/** @} */
