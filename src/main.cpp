#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_string.h"
#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "common_variable_8x16_sprite_font.h"

// Add audio headers
#include "bn_music.h"
#include "bn_music_items.h"
#include "bn_sound_items.h"

// If you have background assets, include them here:
// #include "bn_regular_bg_items_your_background.h"

namespace
{
    // Page states
    enum class page_state
    {
        INTRO,
        MAIN_CAROUSEL,
        EXPERIENCE_DETAIL
    };

    // Main carousel pages
    enum class carousel_page
    {
        EXPERIENCE,
        PROJECTS,
        SKILLS,
        EDUCATION
    };

    // Experience sections
    enum class experience_section
    {
        HAIVISION,
        LAB_STAFF,
        MACHINE_LEARNING
    };

    class resume_game
    {
    private:
        bn::sprite_text_generator _text_generator;
        bn::vector<bn::sprite_ptr, 128> _text_sprites;
        page_state _current_state;
        carousel_page _current_carousel_page;
        experience_section _selected_experience;
        int _highlight_index;
        experience_section _detail_section;

    public:
        resume_game() :
            _text_generator(common::variable_8x16_sprite_font),
            _current_state(page_state::INTRO),
            _current_carousel_page(carousel_page::EXPERIENCE),
            _selected_experience(experience_section::HAIVISION),
            _highlight_index(0)
        {
            // Set up a nice background color scheme
            setup_background();
            
            // Start playing background music
            bn::music_items::cyberrid.play(0.5, true);  // 0.5 volume, true for loop
            
            show_intro_page();
        }

        void update()
        {
            switch(_current_state)
            {
                case page_state::INTRO:
                    update_intro();
                    break;
                case page_state::MAIN_CAROUSEL:
                    update_carousel();
                    break;
                case page_state::EXPERIENCE_DETAIL:
                    update_detail();
                    break;
                default:
                    break;
            }
        }

    private:
        void setup_background()
        {
            // Simple gradient background using palette manipulation
            // This creates a nice blue gradient effect
            bn::bg_palettes::set_transparent_color(bn::color(2, 4, 8));  // Dark blue
            
            // You can experiment with different colors:
            // bn::bg_palettes::set_transparent_color(bn::color(4, 8, 12));  // Lighter blue
            // bn::bg_palettes::set_transparent_color(bn::color(8, 4, 4));   // Dark red
            // bn::bg_palettes::set_transparent_color(bn::color(4, 8, 4));   // Dark green
            // bn::bg_palettes::set_transparent_color(bn::color(12, 12, 12)); // Gray
        }

        void clear_text()
        {
            _text_sprites.clear();
        }

        void show_intro_page()
        {
            clear_text();
            
            _text_generator.set_center_alignment();
            
            _text_generator.generate(0, -60, "SAMUEL CALVERT", _text_sprites);
            _text_generator.generate(0, -40, "Computer Engineering", _text_sprites);
            _text_generator.generate(0, -25, "Professional", _text_sprites);
            
            _text_generator.generate(0, -5, "Contact:", _text_sprites);
            _text_generator.generate(0, 10, "(302) 513-3155", _text_sprites);
            _text_generator.generate(0, 25, "hello@samuelcalvert.com", _text_sprites);
            
            _text_generator.generate(0, 50, "Press START or A", _text_sprites);
        }

        void show_carousel_page()
        {
            clear_text();
            
            // Show navigation hints
            _text_generator.set_center_alignment();
            _text_generator.generate(0, -70, get_page_title(), _text_sprites);
            _text_generator.generate(0, 60, "L/R: Nav | U/D: Sel | A: OK", _text_sprites);
            
            // Show page content
            switch(_current_carousel_page)
            {
                case carousel_page::EXPERIENCE:
                    show_experience_page();
                    break;
                case carousel_page::PROJECTS:
                    show_projects_page();
                    break;
                case carousel_page::SKILLS:
                    show_skills_page();
                    break;
                case carousel_page::EDUCATION:
                    show_education_page();
                    break;
                default:
                    break;
            }
        }

        bn::string_view get_page_title()
        {
            switch(_current_carousel_page)
            {
                case carousel_page::EXPERIENCE: return "EXPERIENCE";
                case carousel_page::PROJECTS: return "PROJECTS";  
                case carousel_page::SKILLS: return "SKILLS";
                case carousel_page::EDUCATION: return "EDUCATION";
                default: return "";
            }
        }

        void show_experience_page()
        {
            _text_generator.set_left_alignment();
            int y_pos = -40;
            
            // Haivision
            if(_highlight_index == 0) {
                _text_generator.generate(-100, y_pos, "> Haivision", _text_sprites);
            } else {
                _text_generator.generate(-100, y_pos, "  Haivision", _text_sprites);
            }
            _text_generator.generate(-90, y_pos + 15, "Tech Support Eng", _text_sprites);
            
            y_pos += 35;
            
            // Lab Staff
            if(_highlight_index == 1) {
                _text_generator.generate(-100, y_pos, "> UD EECIS Dept", _text_sprites);
            } else {
                _text_generator.generate(-100, y_pos, "  UD EECIS Dept", _text_sprites);
            }
            _text_generator.generate(-90, y_pos + 15, "Lab Staff", _text_sprites);
            
            y_pos += 35;
            
            // ML Research
            if(_highlight_index == 2) {
                _text_generator.generate(-100, y_pos, "> UD Machine Learn", _text_sprites);
            } else {
                _text_generator.generate(-100, y_pos, "  UD Machine Learn", _text_sprites);
            }
            _text_generator.generate(-90, y_pos + 15, "Research Intern", _text_sprites);
        }

        void show_projects_page()
        {
            _text_generator.set_left_alignment();
            _text_generator.generate(-100, -30, "Homelab K8s Infra", _text_sprites);
            _text_generator.generate(-90, -10, "- Multi-env clusters", _text_sprites);
            _text_generator.generate(-90, 5, "- Infrastructure Code", _text_sprites);
            _text_generator.generate(-90, 20, "- GitOps methods", _text_sprites);
            _text_generator.generate(-90, 35, "- Prometheus monitor", _text_sprites);
        }

        void show_skills_page()
        {
            _text_generator.set_left_alignment();
            _text_generator.generate(-100, -40, "Infrastructure:", _text_sprites);
            _text_generator.generate(-90, -25, "K8s, Docker, Terraform", _text_sprites);
            
            _text_generator.generate(-100, -5, "Programming:", _text_sprites);
            _text_generator.generate(-90, 10, "Bash, Python, Go", _text_sprites);
            
            _text_generator.generate(-100, 30, "Networking:", _text_sprites);
            _text_generator.generate(-90, 45, "Wireshark, Nmap, GNS3", _text_sprites);
        }

        void show_education_page()
        {
            _text_generator.set_center_alignment();
            _text_generator.generate(0, -20, "University of Delaware", _text_sprites);
            _text_generator.generate(0, 0, "Bachelor of Science", _text_sprites);
            _text_generator.generate(0, 15, "Computer Engineering", _text_sprites);
            _text_generator.generate(0, 35, "Graduated May 2022", _text_sprites);
        }

        void show_experience_detail()
        {
            clear_text();
            _text_generator.set_center_alignment();
            
            switch(_detail_section)
            {
                case experience_section::HAIVISION:
                    _text_generator.generate(0, -60, "HAIVISION", _text_sprites);
                    _text_generator.generate(0, -45, "July 2022 - Present", _text_sprites);
                    _text_generator.set_left_alignment();
                    _text_generator.generate(-110, -20, "- Deploy problem solve", _text_sprites);
                    _text_generator.generate(-110, -5, "- Packer images", _text_sprites);
                    _text_generator.generate(-110, 10, "- 500+ endpoints", _text_sprites);
                    _text_generator.generate(-110, 25, "- Stream optimization", _text_sprites);
                    _text_generator.generate(-110, 40, "- 50Gbps peak traffic", _text_sprites);
                    break;
                    
                case experience_section::LAB_STAFF:
                    _text_generator.generate(0, -60, "LAB STAFF", _text_sprites);
                    _text_generator.generate(0, -45, "Jun 2019 - Aug 2021", _text_sprites);
                    _text_generator.set_left_alignment();
                    _text_generator.generate(-110, -20, "- 25 servers maintain", _text_sprites);
                    _text_generator.generate(-110, -5, "- ESXi VM system", _text_sprites);
                    _text_generator.generate(-110, 10, "- SLURM scheduling", _text_sprites);
                    _text_generator.generate(-110, 25, "- Ansible automation", _text_sprites);
                    _text_generator.generate(-110, 40, "- 100TB datasets", _text_sprites);
                    break;
                    
                case experience_section::MACHINE_LEARNING:
                    _text_generator.generate(0, -60, "ML RESEARCH", _text_sprites);
                    _text_generator.generate(0, -45, "Jun 2020 - Aug 2020", _text_sprites);
                    _text_generator.set_left_alignment();
                    _text_generator.generate(-110, -20, "- Network IDS", _text_sprites);
                    _text_generator.generate(-110, -5, "- GAN neural nets", _text_sprites);
                    _text_generator.generate(-110, 10, "- GNS3 virtual nets", _text_sprites);
                    _text_generator.generate(-110, 25, "- Docker containers", _text_sprites);
                    _text_generator.generate(-110, 40, "- Research docs", _text_sprites);
                    break;
                    
                default:
                    break;
            }
            
            _text_generator.set_center_alignment();
            _text_generator.generate(0, 65, "Press B to go back", _text_sprites);
        }

        void update_intro()
        {
            if(bn::keypad::start_pressed() || bn::keypad::a_pressed())
            {
                bn::sound_items::alert.play();
                _current_state = page_state::MAIN_CAROUSEL;
                _highlight_index = 0;
                show_carousel_page();
            }
        }

        void update_carousel()
        {
            // Handle B button to go back to intro
            if(bn::keypad::b_pressed())
            {
                bn::sound_items::alert.play();
                _current_state = page_state::INTRO;
                show_intro_page();
                return;  // Exit early to avoid processing other inputs
            }

            // Navigation between pages
            if(bn::keypad::left_pressed())
            {
                bn::sound_items::alert.play();
                int page_index = static_cast<int>(_current_carousel_page);
                page_index = (page_index - 1 + 4) % 4;
                _current_carousel_page = static_cast<carousel_page>(page_index);
                _highlight_index = 0;
                show_carousel_page();
            }
            else if(bn::keypad::right_pressed())
            {
                bn::sound_items::alert.play();
                int page_index = static_cast<int>(_current_carousel_page);
                page_index = (page_index + 1) % 4;
                _current_carousel_page = static_cast<carousel_page>(page_index);
                _highlight_index = 0;
                show_carousel_page();
            }
            
            // Handle experience page selection
            if(_current_carousel_page == carousel_page::EXPERIENCE)
            {
                if(bn::keypad::up_pressed())
                {
                    bn::sound_items::alert.play();
                    _highlight_index = (_highlight_index - 1 + 3) % 3;
                    show_carousel_page();
                }
                else if(bn::keypad::down_pressed())
                {
                    bn::sound_items::alert.play();
                    _highlight_index = (_highlight_index + 1) % 3;
                    show_carousel_page();
                }
                
                if(bn::keypad::a_pressed())
                {
                    bn::sound_items::alert.play();
                    _current_state = page_state::EXPERIENCE_DETAIL;
                    _detail_section = static_cast<experience_section>(_highlight_index);
                    show_experience_detail();
                }
            }
        }

        void update_detail()
        {
            if(bn::keypad::b_pressed())
            {
                bn::sound_items::alert.play();
                _current_state = page_state::MAIN_CAROUSEL;
                show_carousel_page();
            }
        }
    };
}

int main()
{
    bn::core::init();
    
    resume_game game;
    
    while(true)
    {
        game.update();
        bn::core::update();
    }
}