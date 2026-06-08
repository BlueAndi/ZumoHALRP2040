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
 *
 * The note sequence parser and playback handling are based on the Pololu
 * buzzer implementation and adapted for the Zumo 2040.
 * https://github.com/pololu/pololu-buzzer-arduino/tree/master
 *
 * @author Felix Reitenauer
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Zumo2040Buzzer.h"
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/clocks.h>
/******************************************************************************
 * Compiler Switches
 *****************************************************************************/

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and classes
 *****************************************************************************/

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/** Mask which is used to check if DIV_BY_10 bit is set. */
constexpr uint16_t DIV_BY_10_MASK = (1u<<15u);
/** Unsigned 8-bit integer zero. */
constexpr uint8_t U_INTEGER_8_ZERO = 0u;
/** Unsigned 16-bit integer zero. */
constexpr uint16_t U_INTEGER_16_ZERO = 0u;
/** Unsigned 32-bit integer zero. */
constexpr uint32_t U_INTEGER_32_ZERO = 0u;
/** Unsigned 32-bit integer one. */
constexpr uint32_t U_INTEGER_32_ONE = 1u;
/** Unsigned 8-bit one. */
constexpr uint8_t U_INTEGER_8_ONE = 1u;
/** Default alarm id if no alarm is active. */
constexpr alarm_id_t ALARM_ID_ZERO = 0;
/** PWM slice which the buzzer uses. */
constexpr uint8_t BUZZER_PWM_SLICE = 3u;
/** PWM slice channel the buzzer uses. */
constexpr uint8_t BUZZER_CHAN = 1u;
/** Maximum volume which can be used. */
constexpr uint32_t MAX_VOLUME = 15u;
/** Dummy frequency used for the silent note. */
constexpr uint16_t DUMMY_FREQ = 1000u;
/** Value which sets the volume to zero. */
constexpr uint16_t VOLUME_ZERO = 0u;
/** Value for notes which is used if a silent note shall be played. */
constexpr uint8_t SILENT_NOTE = 255u;
/** Maximal allowed note offset from E1. */
constexpr uint8_t MAX_NOTE_OFFSET = 95u;
/** Notes per octave.  */
constexpr uint8_t NOTES_PER_OCTAVE = 12u;
/** Maximum exponent which is allowed. */
constexpr uint8_t MAX_EXPONENT = 7u;
/** Is used to multiply by 64. */
constexpr uint16_t MUL_BY_64 = 64u;
/** Divider used to convert from 0.1 Hz to Hz. */
constexpr uint16_t FREQ_TENTHS_SCALE = 10u;
/** Offset used for correct rounding in the frequency calculation. */
constexpr uint16_t FREQ_TENTHS_ROUNDING_OFFSET = FREQ_TENTHS_SCALE / 2u;
/** Offset used for correct rounding in the frequency calculation. */
constexpr uint16_t ROUNDING_OFFSET_2 = 2u;
/** Integer five used for dividing. */
constexpr uint16_t DIV_BY_5_I = 5u;
/** Integer two used for dividing. */
constexpr uint32_t DIV_BY_2_I = 2u;
/** Used to shift decimal values to the left. */
constexpr uint32_t DECIMAL_BASE = 10u;
/** Count of milliseconds in a minute. */
constexpr uint32_t MS_PER_MINUTE = 60000u;
/** Count of quarter notes per whole note. */
constexpr uint32_t QUARTER_NOTES_PER_WHOLE_NOTE = 4u;
/** Default octave. */
constexpr uint32_t DEFAULT_OCTAVE = 4u;
/** Default whole note duration. */
constexpr uint32_t DEFAULT_WHOLE_NOTE_DURATION = 2000u;
/** Default note type (quarter note). */
constexpr uint32_t DEFAULT_NOTE_TYPE = 4u;
/** Default duration for a note. */
constexpr uint32_t DEFAULT_DURATION = 500u;
/** Default buzzer volume. */
constexpr uint16_t DEFAULT_VOLUME = 15u;
/** Maximum octave which can be played. */
constexpr uint32_t MAX_OCTAVE = 9u;
/** Float 2 used for dividing. */
constexpr float DIV_BY_2_FL = 2.0F;
/** Maximum possible wrap count. */
constexpr float MAX_WRAP_COUNT = 65536.0F;
/** Float zero. */
constexpr float FLOAT_ZERO = 0.0F;
/** Float one. */
constexpr float FLOAT_ONE = 1.0F;
/** Float ten used for dividing. */
constexpr float DIV_BY_10_FL = 10.0F;
/** Maximum value for the clock divider. */
constexpr float MAX_DIV = 255.9375F;
/** Half of the clock divider step used for rounding. */
constexpr float CLK_DIV_ROUNDING_OFFSET = 0.03125F;
/** Rounding offset used for float to integer conversion.  */
constexpr float ROUNDING_OFFSET = 0.5F;
/** Note offset from E1 for E1. */
constexpr uint8_t E1_OFFSET = 0;
/** Note offset from E1 for F1. */
constexpr uint8_t F1_OFFSET = 1;
/** Note offset from E1 for F#1. */
constexpr uint8_t F_SHARP_1_OFFSET = 2;
/** Note offset from E1 for G1. */
constexpr uint8_t G1_OFFSET = 3;
/** Note offset from E1 for G#1. */
constexpr uint8_t G_SHARP_1_OFFSET = 4;
/** Note offset from E1 for A1. */
constexpr uint8_t A1_OFFSET = 5;
/** Note offset from E1 for A#1. */
constexpr uint8_t A_SHARP_1_OFFSET = 6;
/** Note offset from E1 for B1. */
constexpr uint8_t B1_OFFSET = 7;
/** Note offset from E1 for C2. */
constexpr uint8_t C2_OFFSET = 8;
/** Note offset from E1 for C#2. */
constexpr uint8_t C_SHARP_2_OFFSET = 9;
/** Note offset from E1 for D2. */
constexpr uint8_t D2_OFFSET = 10;
/** Note offset from E1 for D#2. */
constexpr uint8_t D_SHARP_2_OFFSET = 11;
/** Numerical value for the note E1. */
constexpr uint8_t NOTE_E1 = 16u;
/** Numerical value for the note D#9. */
constexpr uint8_t NOTE_D_SHARP_9 = 111u;
/** Frequency for the note E1 in 0.1 Hz. */
constexpr uint16_t FREQ_E1 = 412u;
/** Frequency for the note F1 in 0.1 Hz. */
constexpr uint16_t FREQ_F1 = 437u;
/** Frequency for the note F#1 in 0.1 Hz. */
constexpr uint16_t FREQ_F_SHARP_1 = 463u;
/** Frequency for the note G1 in 0.1 Hz. */
constexpr uint16_t FREQ_G1 = 490u;
/** Frequency for the note G#1 in 0.1 Hz. */
constexpr uint16_t FREQ_G_SHARP_1 = 519u;
/** Frequency for the note A1 in 0.1 Hz. */
constexpr uint16_t FREQ_A1 = 550u;
/** Frequency for the note A#1 in 0.1 Hz. */
constexpr uint16_t FREQ_A_SHARP_1 = 583u;
/** Frequency for the note B1 in 0.1 Hz. */
constexpr uint16_t FREQ_B1 = 617u;
/** Frequency for the note C2 in 0.1 Hz. */
constexpr uint16_t FREQ_C2 = 654u;
/** Frequency for the note C#2 in 0.1 Hz. */
constexpr uint16_t FREQ_C_SHARP_2 = 693u;
/** Frequency for the note D2 in 0.1 Hz. */
constexpr uint16_t FREQ_D2 = 734u;
/** Frequency for the note D#2 in 0.1 Hz. */
constexpr uint16_t FREQ_D_SHARP_2 = 778u;
/** Note key for the note C. */
constexpr uint8_t NOTE_KEY_C       = 0u;
/** Note key for the note C#. */
constexpr uint8_t NOTE_KEY_C_SHARP = 1u;
/** Note key for the note D. */
constexpr uint8_t NOTE_KEY_D       = 2u;
/** Note key for the note D#. */
constexpr uint8_t NOTE_KEY_D_SHARP = 3u;
/** Note key for the note E. */
constexpr uint8_t NOTE_KEY_E       = 4u;
/** Note key for the note F. */
constexpr uint8_t NOTE_KEY_F       = 5u;
/** Note key for the note F#. */
constexpr uint8_t NOTE_KEY_F_SHARP = 6u;
/** Note key for the note G. */
constexpr uint8_t NOTE_KEY_G       = 7u;
/** Note key for the note G#. */
constexpr uint8_t NOTE_KEY_G_SHARP = 8u;
/** Note key for the note A. */
constexpr uint8_t NOTE_KEY_A       = 9u;
/** Note key for the note A#. */
constexpr uint8_t NOTE_KEY_A_SHARP = 10u;
/** Note key for the note B. */
constexpr uint8_t NOTE_KEY_B       = 11u;
/** Indicates that the next note to be played is not a silent note. */
constexpr bool NO_REST = false;
/** Indicates that the next note to be played is a silent note. */
constexpr bool REST = true;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

Zumo2040Buzzer::Zumo2040Buzzer() : m_buzzerSequence(nullptr),
                                   m_buzzerStatus(BUZZER_DISABLED),
                                   m_alarmStatus(Zumo2040::AlarmStatus::ALARM_DISABLED),
                                   m_alarmID(ALARM_ID_ZERO),
                                   m_playmode(PLAY_AUTOMATIC),
                                   m_error(NONE),
                                   m_octave(DEFAULT_OCTAVE),
                                   m_wholeNoteDuration(DEFAULT_WHOLE_NOTE_DURATION),
                                   m_noteType(DEFAULT_NOTE_TYPE),
                                   m_duration(DEFAULT_DURATION),
                                   m_volume(DEFAULT_VOLUME),
                                   m_staccato(STACCATO_DISABLED),
                                   m_staccatoRestDuration(U_INTEGER_32_ZERO)
{
}

ErrorCode Zumo2040Buzzer::playMode(BuzzerPlaymode mode)
{
    m_playmode = mode;

    if ((PLAY_AUTOMATIC == mode) && (BUZZER_DISABLED == m_buzzerStatus))
    {
        return nextNote();
    }

    return m_error;
}

BuzzerStatus Zumo2040Buzzer::playCheck()
{
    if ((BUZZER_DISABLED == m_buzzerStatus) && (nullptr != m_buzzerSequence) && (DONT_PLAY_AUTOMATIC == m_playmode))
    {
        ErrorCode error = nextNote();

        if (NONE != error)
        {
            stopPlaying();
            return BUZZER_DISABLED;
        }
    }

    return (nullptr != m_buzzerSequence) ? BUZZER_ENABLED : BUZZER_DISABLED;
}

ErrorCode Zumo2040Buzzer::playFrequency(uint16_t freq, uint32_t dur, uint16_t volume)
{
    if (NONE != m_error)
    {
        return m_error;
    }

    if (U_INTEGER_16_ZERO == freq)
    {
        m_error = BUZZER_INVALID_FREQ_ZERO;
        return m_error;
    }
    else if (U_INTEGER_32_ZERO == dur)
    {
        m_error = BUZZER_INVALID_DURATION_ZERO;
        return m_error;
    }

    float flFreq = FLOAT_ZERO;
    float clkDiv = FLOAT_ONE;
    float clk = clock_get_hz(clk_sys);
    float flWrapCount = FLOAT_ZERO;
    uint16_t level = U_INTEGER_16_ZERO;
    uint16_t wrap = U_INTEGER_16_ZERO;

    /* Check whether the MSB is set to determine if the frequency is given in 0.1 Hz or Hz. */
    if (DIV_BY_10_MASK == (freq & DIV_BY_10_MASK))
    {
        /* Set the DIV_BY_10 bit to zero. */
        freq &= ~DIV_BY_10_MASK;
        if (U_INTEGER_16_ZERO == freq)
        {
            m_error = BUZZER_INVALID_FREQ_ZERO;
            return m_error;
        }

        flFreq = freq;
        /* Frequency has to be interpreted in 0.1 Hz so we have to divide it by 10. */
        flFreq /= DIV_BY_10_FL;
        flWrapCount = clk / (clkDiv * flFreq);
    }
    else
    {
        /* Frequency has to be interpreted in Hz. */
        flFreq = freq;
        flWrapCount = clk / (clkDiv * flFreq);
    }

    /* Intermediate value for the wrap used to check if we also need a clock divider to reach
     * the intended PWM frequency.
     */
    uint32_t wrapCalc = (flWrapCount - FLOAT_ONE) + ROUNDING_OFFSET;

    /* The PWM frequency is calculated as:
     * freq = clk / (clkDiv * (wrap + 1))
     *
     * If the calculated wrap value is larger than UINT16_MAX, the clock divider
     * is increased and the wrap value is limited to UINT16_MAX.
     */
    if (UINT16_MAX < wrapCalc)
    {
        clkDiv = flWrapCount / MAX_WRAP_COUNT;

        if (MAX_DIV < clkDiv)
        {
            m_error = BUZZER_INVALID_FREQ_TOO_SMALL;
            return m_error;
        }

        /* Add the rounding offset only after the range check. This avoids rejecting a
         * divider that is valid before rounding and only exceeds MAX_DIV due to the
         * rounding offset.
         */
        clkDiv += CLK_DIV_ROUNDING_OFFSET;

        /* We have to check the clock divider again to be sure that it isn't bigger than the maximum after adding
         * the rounding offset.
         */
        if (MAX_DIV < clkDiv)
        {
            clkDiv = MAX_DIV;
        }

        wrapCalc = UINT16_MAX;
        flWrapCount = MAX_WRAP_COUNT;
    }

    wrap = wrapCalc;

    /* Set the counter compare value to the half of the PWM wrap which corresponds to the maximum volume.
     * We do this to decrease the volume later by bit shifting.
     */
    level = (flWrapCount / DIV_BY_2_FL) + ROUNDING_OFFSET;

    if (MAX_VOLUME < volume)
    {
        volume = MAX_VOLUME;
    }
    else if (U_INTEGER_16_ZERO == volume)
    {
        level = U_INTEGER_16_ZERO;
    }
    else
    {
        /* For each decrease in volume the volume gets decreased by 50%. */
        level >>= (MAX_VOLUME - volume);
    }

    /* Cancel an active alarm before starting a new tone. */
    if (Zumo2040::AlarmStatus::ALARM_ENABLED == m_alarmStatus)
    {
        alarm_id_t oldID = m_alarmID;
        m_alarmID = ALARM_ID_ZERO;
        m_alarmStatus = Zumo2040::AlarmStatus::ALARM_DISABLED;
        cancel_alarm(oldID);
    }

    /* Configure the buzzer pin so it can be used for PWM. */
    gpio_set_function(Zumo2040::Pins::BUZZER_PIN, GPIO_FUNC_PWM);
    /* Configure the clock divider to reach the wanted frequency. */
    pwm_set_clkdiv(BUZZER_PWM_SLICE, clkDiv);
    /* Configure the wrap value of the PWM to reach the wanted frequency. */
    pwm_set_wrap(BUZZER_PWM_SLICE, wrap);
    /* Configure the counter compare value to set the wanted volume. */
    pwm_set_chan_level(BUZZER_PWM_SLICE, BUZZER_CHAN, level);
    /* Enable the PWM. */
    pwm_set_enabled(BUZZER_PWM_SLICE, Zumo2040::Status::ENABLED);
    m_buzzerStatus = BUZZER_ENABLED;

    /* Create the alarm that calls the callback function timerDone() after
     * the specified duration. The callback then disables the buzzer output.
     */
    m_alarmID = add_alarm_in_ms(dur, timerDone, this, true );

    /* If the alarm could not be created the function returns a value < 0. */
    if (m_alarmID < ALARM_ID_ZERO)
    {
        pwm_set_enabled(BUZZER_PWM_SLICE, Zumo2040::Status::DISABLED);

        m_buzzerStatus = BUZZER_DISABLED;
        m_alarmStatus = Zumo2040::AlarmStatus::ALARM_DISABLED;
        m_alarmID = ALARM_ID_ZERO;

        m_error = BUZZER_COULD_NOT_SET_ALARM;
        return m_error;
    }

    m_alarmStatus = Zumo2040::AlarmStatus::ALARM_ENABLED;

    return NONE;
}

ErrorCode Zumo2040Buzzer::play(const char* notes)
{
    if (NONE != m_error)
    {
        return m_error;
    }

    if (nullptr == notes)
    {
        m_error = BUZZER_INVALID_MELODY_NULLPTR;
        return m_error;
    }
    m_buzzerSequence = notes;
    /* Reset the staccato rest duration to be sure there are no old temporary values. */
    m_staccatoRestDuration = U_INTEGER_32_ZERO;

    if (PLAY_AUTOMATIC == m_playmode)
    {
        return nextNote();
    }

    return m_error;
}

void Zumo2040Buzzer::stopPlaying()
{
    /* Stop the output of the buzzer. */
    pwm_set_chan_level(BUZZER_PWM_SLICE, BUZZER_CHAN, U_INTEGER_16_ZERO);
    pwm_set_enabled(BUZZER_PWM_SLICE, Zumo2040::Status::DISABLED);
    gpio_set_function(Zumo2040::Pins::BUZZER_PIN, GPIO_FUNC_SIO);

    if (Zumo2040::AlarmStatus::ALARM_ENABLED == m_alarmStatus)
    {
        cancel_alarm(m_alarmID);
    }

    m_buzzerStatus = BUZZER_DISABLED;
    m_buzzerSequence = nullptr;
    m_alarmStatus = Zumo2040::AlarmStatus::ALARM_DISABLED;
    m_alarmID = ALARM_ID_ZERO;
    m_staccatoRestDuration = U_INTEGER_32_ZERO;
}

bool Zumo2040Buzzer::isPlaying() const
{
    return ((BUZZER_ENABLED == m_buzzerStatus) || (nullptr != m_buzzerSequence));
}

ErrorCode Zumo2040Buzzer::getLastError() const
{
    return m_error;
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

int64_t Zumo2040Buzzer::timerDone(alarm_id_t id, void* user_data)
{
    if (nullptr == user_data)
    {
        return 0;
    }

    Zumo2040Buzzer* buzzer = static_cast<Zumo2040Buzzer*>(user_data);

    if (id != buzzer->m_alarmID)
    {
        return 0;
    }

    pwm_set_chan_level(BUZZER_PWM_SLICE, BUZZER_CHAN, U_INTEGER_16_ZERO);
    pwm_set_enabled(BUZZER_PWM_SLICE, Zumo2040::Status::DISABLED);

    buzzer->m_buzzerStatus = BUZZER_DISABLED;
    buzzer->m_alarmStatus = Zumo2040::AlarmStatus::ALARM_DISABLED;
    buzzer->m_alarmID = ALARM_ID_ZERO;

    if ((buzzer->m_buzzerSequence) && (buzzer->m_playmode == PLAY_AUTOMATIC))
    {
        buzzer->nextNote();
    }

    return 0;
}

ErrorCode Zumo2040Buzzer::playNote(uint8_t note, uint32_t dur, uint16_t volume)
{
    if (NONE != m_error)
    {
        return m_error;
    }

    if ((note == SILENT_NOTE) || (U_INTEGER_16_ZERO == volume))
    {
        return playFrequency(DUMMY_FREQ, dur, VOLUME_ZERO);
    }

    /* This value represents the numerical offset of the corresponding note from
     * the note E1. The numerical value of a note gets calculated as in the
     * following: Note_key + octave * 12
     * For example E1 has the Note key = 4 and the octave = 1:
     * 4 + 1 * 12 = 16
     * Because E1 is the smallest allowed note we use 16 as offset
     * for all numerical note values.
     */
    uint8_t offsetNote = U_INTEGER_8_ZERO;
    uint16_t freq = U_INTEGER_16_ZERO;

    if (NOTE_E1 >= note)
    {
        offsetNote = U_INTEGER_8_ZERO;
    }
    /* D#9 is the highest allowed note (~10 kHz). */
    else if (NOTE_D_SHARP_9 <= note)
    {
        offsetNote = MAX_NOTE_OFFSET;
    }
    else
    {
        offsetNote = note - NOTE_E1;
    }

    /* Determine the exponent which indicates if the frequency of the note is 2^x times higher than
     * the frequency of the following base notes.
     */
    uint8_t exponent = offsetNote / NOTES_PER_OCTAVE;

    /* If we divide by the count of notes the rest indicates which base note we have to use. */
    switch (offsetNote % NOTES_PER_OCTAVE)
    {
        case E1_OFFSET:
            freq = FREQ_E1;
            break;
        case F1_OFFSET:
            freq = FREQ_F1;
            break;
        case F_SHARP_1_OFFSET:
            freq = FREQ_F_SHARP_1;
            break;
        case G1_OFFSET:
            freq = FREQ_G1;
            break;
        case G_SHARP_1_OFFSET:
            freq = FREQ_G_SHARP_1;
            break;
        case A1_OFFSET:
            freq = FREQ_A1;
            break;
        case A_SHARP_1_OFFSET:
            freq = FREQ_A_SHARP_1;
            break;
        case B1_OFFSET:
            freq = FREQ_B1;
            break;
        case C2_OFFSET:
            freq = FREQ_C2;
            break;
        case C_SHARP_2_OFFSET:
            freq = FREQ_C_SHARP_2;
            break;
        case D2_OFFSET:
            freq = FREQ_D2;
            break;
        case D_SHARP_2_OFFSET:
            freq = FREQ_D_SHARP_2;
            break;

        default:
            break;
    }

    /* We can only increase the base values by 7 octaves. */
    if (MAX_EXPONENT > exponent)
    {
        /* The frequency of every note doubles for every octave. */
        freq <<= exponent;

        /* For higher octaves, convert the frequency from 0.1 Hz to Hz. */
        if (exponent > U_INTEGER_8_ONE)
        {
            freq = (freq + FREQ_TENTHS_ROUNDING_OFFSET) / FREQ_TENTHS_SCALE;
        }
        else
        {
            /* Keep the 0.1 Hz resolution for low frequencies. */
            freq |= DIV_BY_10_MASK;
        }
    }
    else
    {
        /* Convert the frequency from 0.1 Hz to Hz.
         * The calculation is scaled to avoid an overflow.
         */
        freq = ((freq * MUL_BY_64) + ROUNDING_OFFSET_2) / DIV_BY_5_I;
    }

    if (MAX_VOLUME < volume)
    {
        volume = MAX_VOLUME;
    }

    return playFrequency(freq, dur, volume);
}

char Zumo2040Buzzer::currentCharacter()
{
    char c = U_INTEGER_8_ZERO;

    if (nullptr == m_buzzerSequence)
    {
        return c;
    }

    do
    {
        c = *m_buzzerSequence;
        /* Convert capital letters to lower case letters. */
        if ('A' <= c && 'Z' >= c)
            c += 'a' - 'A';
    } while ((' ' == c) && m_buzzerSequence++);

    return c;
}

uint32_t Zumo2040Buzzer::getNumber()
{
    uint32_t num = U_INTEGER_32_ZERO;

    char c = currentCharacter();
    /* Convert ASCII digits to numeric values. */
    while (c >= '0' && c <= '9')
    {
        num *= DECIMAL_BASE;
        num += c - '0';
        m_buzzerSequence++;
        c = currentCharacter();
    }

    return num;
}

ErrorCode Zumo2040Buzzer::nextNote()
{
    if (NONE != m_error)
    {
        return m_error;
    }

    if (nullptr == m_buzzerSequence)
    {
        return NONE;
    }

    bool rest = NO_REST;
    uint8_t note = U_INTEGER_8_ZERO;
    uint32_t tmpOctave = m_octave;
    uint32_t tmp_duration = U_INTEGER_32_ZERO;
    uint32_t tmpVol = U_INTEGER_32_ZERO;
    uint32_t dot_add = U_INTEGER_32_ZERO;
    uint32_t tempo = U_INTEGER_32_ZERO;
    uint32_t noteDurationDivisor = U_INTEGER_32_ONE;

    char c = U_INTEGER_8_ZERO;

    if (m_staccato && (U_INTEGER_32_ZERO != m_staccatoRestDuration))
    {
        playNote(SILENT_NOTE, m_staccatoRestDuration, VOLUME_ZERO);
        m_staccatoRestDuration = U_INTEGER_32_ZERO;
        return m_error;
    }

    parse_character:

    c = currentCharacter();
    m_buzzerSequence++;

    switch (c)
    {
        /* Raise the next note by one octave. */
        case '>':
            if (MAX_OCTAVE > tmpOctave)
            {
                tmpOctave++;
            }

            goto parse_character;
        /* Lower the next note by one octave. */
        case '<':
            if (U_INTEGER_32_ZERO < tmpOctave)
            {
                tmpOctave--;
            }

            goto parse_character;
        /* Set the default note length. */
        case 'l':
            noteDurationDivisor = getNumber();

            if (U_INTEGER_32_ZERO != noteDurationDivisor)
            {
                m_noteType = noteDurationDivisor;
                m_duration = m_wholeNoteDuration / m_noteType;
            }

            goto parse_character;
        /* Set the staccato mode. */
        case 'm':

            c = currentCharacter();
            if ('l' == c)
            {
                m_staccato = STACCATO_DISABLED;
                m_buzzerSequence++;
            }
            else if ('s' == c)
            {
                m_staccato = STACCATO_ENABLED;
                m_staccatoRestDuration = U_INTEGER_32_ZERO;
                m_buzzerSequence++;
            }
            goto parse_character;
        /* Set the default octave. */
        case 'o':
            tmpOctave = getNumber();

            if (MAX_OCTAVE < tmpOctave)
            {
                tmpOctave = MAX_OCTAVE;
            }

            m_octave = tmpOctave;
            goto parse_character;
        /* Insert a rest. */
        case 'r':
            rest = REST;
            break;
        /* Set the tempo in beats per minute.
         * Each beat is defined as a quarter note.
         */
        case 't':
            tempo = getNumber();
            if (U_INTEGER_32_ZERO != tempo)
            {
                m_wholeNoteDuration = (MS_PER_MINUTE * QUARTER_NOTES_PER_WHOLE_NOTE) / tempo;
                m_duration = m_wholeNoteDuration / m_noteType;
            }
            goto parse_character;
        /* Set the volume. */
        case 'v':
            tmpVol = getNumber();

            if (MAX_VOLUME < tmpVol)
            {
                m_volume = MAX_VOLUME;
            }
            else
            {
                m_volume = tmpVol;
            }

            goto parse_character;
        /* Restore the persistent playback settings to the default values. */
        case '!':
            m_octave = DEFAULT_OCTAVE;
            m_wholeNoteDuration = DEFAULT_WHOLE_NOTE_DURATION;
            m_noteType = DEFAULT_NOTE_TYPE;
            m_duration = DEFAULT_DURATION;
            m_volume = DEFAULT_VOLUME;
            m_staccato = STACCATO_DISABLED;
            m_staccatoRestDuration = U_INTEGER_32_ZERO;

            /* Reset the temporary variables that depend on the defaults. */
            tmpOctave = m_octave;
            goto parse_character;
        /* Set the base note to A. */
        case 'a':
            note = NOTE_KEY_A;
            break;
        /* Set the base note to B. */
        case 'b':
            note = NOTE_KEY_B;
            break;
        /* Set the base note to C. */
        case 'c':
            note = NOTE_KEY_C;
            break;
        /* Set the base note to D. */
        case 'd':
            note = NOTE_KEY_D;
            break;
        /* Set the base note to E. */
        case 'e':
            note = NOTE_KEY_E;
            break;
        /* Set the base note to F. */
        case 'f':
            note = NOTE_KEY_F;
            break;
        /* Set the base note to G. */
        case 'g':
            note = NOTE_KEY_G;
            break;

        default:
            m_buzzerSequence = nullptr;
            return NONE;
    }

    note += tmpOctave * NOTES_PER_OCTAVE;

    c = currentCharacter();

    while (('+' == c) || ('#' == c))
    {
        /* Raise the next note by one semitone. */
        m_buzzerSequence++;

        if (NOTE_D_SHARP_9 > note)
        {
            note++;
        }

        c = currentCharacter();
    }

    while ('-' == c)
    {
        /* Lower the next note by one semitone. */
        m_buzzerSequence++;

        if (U_INTEGER_8_ZERO < note)
        {
            note--;
        }

        c = currentCharacter();
    }

    tmp_duration = m_duration;

    if (('0' < c) && ('9' >= c))
    {
        noteDurationDivisor = getNumber();

        if (U_INTEGER_32_ZERO != noteDurationDivisor)
        {
            /* Calculate the length for the next note. */
            tmp_duration = m_wholeNoteDuration / noteDurationDivisor;
        }
    }

    /* Each dot increases the note duration by half of its current value. */
    dot_add = tmp_duration / DIV_BY_2_I;
    while (currentCharacter() == '.')
    {
        m_buzzerSequence++;
        tmp_duration += dot_add;
        dot_add /= DIV_BY_2_I;
    }

    if (STACCATO_ENABLED == m_staccato)
    {
        m_staccatoRestDuration = tmp_duration / DIV_BY_2_I;
        tmp_duration -= m_staccatoRestDuration;
    }

    return playNote((rest ? SILENT_NOTE : note), tmp_duration, m_volume);
}

/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/
