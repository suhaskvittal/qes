/*  
 *  author: Suhas Vittal
 *  date:   5 March 2024
 * */

#include "qes/lang/fast_parse.h"
#include "qes/lang/fast_parse_impl.h"

#include <ctype.h>

namespace qes {

inline void
raise_syntax_error(Token tok, const debug_state_t& st) {
    std::cerr << "[ qes ] found invalid token \"" << std::get<1>(tok) << "\" of type "
        << std::get<0>(tok) << " at " << st << std::endl;
    exit(1);
}

Program<>
fast_read_program(std::istream& fin) {
    debug_state_t st = {0, 0};
    return read_block(fin, st);
}

Program<>
read_block(std::istream& fin, debug_state_t& st) {
    status_t status = status_t::awaiting_token;
    parse_state_t p_st;

    Token tok;
    do {
        tok = read_next_token(fin, st);
        token_type token_type = std::get<0>(tok);
        std::string token_val = std::get<1>(tok);
        if (token_type == T_empty) continue;
        if (token_type == T_undefined) break;
        // Parse the token.
        std::cout << "Found token " << token_val << " of type " << token_type << std::endl;
        std::cout << "status = " << ((int) status) << std::endl;
        if (status == status_t::awaiting_token) {
            status = parse_awaiting_token(token_type, token_val, p_st);
        } else if (status == status_t::in_instruction) {
            status = parse_in_instruction(token_type, token_val, p_st);
        } else if (status == status_t::awaiting_modifier) {
            status = parse_awaiting_modifier(token_type, p_st);
        } else if (status == status_t::in_annotation) {
            status = parse_in_annotation(token_type, token_val, p_st);
        } else if (status == status_t::in_property) {
            status = parse_in_property(token_type, token_val, p_st);
        } else if (status == status_t::in_label) {
            status = parse_in_label(token_type, token_val, p_st);
        } else if (status == status_t::in_repeat) {
            status = parse_in_repeat(token_type, token_val, p_st);
        }
        // Handle status result.
        if (status == status_t::invalid) {
            raise_syntax_error(tok, st);
        } else if (status == status_t::exit_block) {
            break;
        } else if (status == status_t::enter_subblock) {
            Program<> blk = read_block(fin, st);
            // Push back blk as many times as specified by repeat.
            while (p_st.repeat_ctr--) {
                p_st.program.insert(p_st.program.end(), blk.begin(), blk.end());
            }
            status = status_t::awaiting_token;
        }
    } while (std::get<0>(tok) != T_undefined);
    return p_st.program;
}

Token
read_next_token(std::istream& fin, debug_state_t& st) {
    const std::set<char> SPECIAL_CHARS{
        ',', ':', ';', '(', ')', '{', '}', '@'
    };
    const std::set<std::string> KEYWORDS{
        "repeat",
        "annotation",
        "property"
    };

    const int found_none = 0,
                found_attr = 1,
                found_identifier = 2,
                found_integer = 3,
                found_float = 4,
                found_string = 5,
                found_comment = 6;

    int status = found_none;
    std::string token_type = T_empty;
    std::string tok;
    while (!fin.eof()) {
        char c = fin.get();
        if (c == EOF) {
            return std::make_tuple(T_undefined, "");
        }
        // Update the debug state:
        if (c == '\n') {
            st.line++;
            st.col = 0;
        } else {
            st.col++;
        }
        // Just keep consuming characters if there is a comment. Only
        // stop when we find a newline.
        if (status == found_comment) {
            if (c == '\n') break;
            else continue;
        }
        // Prioritize string literals, which match anything between their quotes.
        if (status == found_string) {
            tok.push_back(c);
            if (c == '\"') {
                break;
            }
        } else if ((isalpha(c) || c == '_') && status == found_none) {
            if (isupper(c)) c += 'a' - 'A';
            tok.push_back(c);
            token_type = "IDENTIFIER";
            status = found_identifier;
        } else if ((isalpha(c) || isdigit(c) || c == '_') && status == found_identifier) {
            tok.push_back(c);
        } else if (isdigit(c) && 
                (status == found_none || status == found_integer || status == found_float)) 
        {
            tok.push_back(c);
            if (status == found_none) {
                token_type = "I_LITERAL";
                status = found_integer;
            }
        } else if (status == found_none && c == '\"') {
            tok.push_back(c);
            token_type = "S_LITERAL";
            status = found_string;
        } else if (isspace(c)) {
            break;
        } else if (c == '#') {
            status = found_comment;
        } else if (c == '.' && (status == found_none || status == found_integer)) {
            tok.push_back(c);
            token_type = "F_LITERAL";
            status = found_float;
        } else if (status == found_none && SPECIAL_CHARS.count(c)) {
            token_type = std::string{c};
            break;
        } else {
            // If we find a character that does not match the current token type,
            // then place back the char and exit.
            if (status == found_none) {
                std::cerr << "[ qes ] invalid character \"" << c << "\" detected at " << st << std::endl;
                exit(1);
            }
            fin.unget();
            break;
        }
    }
    if (fin.eof()) return std::make_tuple(T_undefined, "");
    // Post process the information. If we found a keyword, then set the
    // token type accordingly.
    if (KEYWORDS.count(tok)) {
        token_type = tok;
    }
    return std::make_tuple(token_type, tok);
}

}   // qes
