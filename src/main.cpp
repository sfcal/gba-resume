#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_string.h"
#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_regular_bg_ptr.h"
#include "bn_timer.h"
#include "bn_sound.h"
#include "bn_math.h"
#include "common_variable_8x16_sprite_font.h"
#include "common_variable_8x8_sprite_font.h"

// Audio headers
#include "bn_music.h"
#include "bn_music_items.h"
#include "bn_sound_items.h"

// Background headers
#include "bn_regular_bg_items_main_background.h"

// Sprite headers
#include "bn_sprite_items_arrow_left.h"
#include "bn_sprite_items_arrow_right.h"

// Intro frame headers
#include "bn_regular_bg_items_frame_0001.h"
#include "bn_regular_bg_items_frame_0002.h"
#include "bn_regular_bg_items_frame_0003.h"
#include "bn_regular_bg_items_frame_0004.h"
#include "bn_regular_bg_items_frame_0005.h"
#include "bn_regular_bg_items_frame_0006.h"
#include "bn_regular_bg_items_frame_0007.h"
#include "bn_regular_bg_items_frame_0008.h"
#include "bn_regular_bg_items_frame_0009.h"
#include "bn_regular_bg_items_frame_0010.h"
#include "bn_regular_bg_items_frame_0011.h"
#include "bn_regular_bg_items_frame_0012.h"
#include "bn_regular_bg_items_frame_0013.h"
#include "bn_regular_bg_items_frame_0014.h"
#include "bn_regular_bg_items_frame_0015.h"
#include "bn_regular_bg_items_frame_0016.h"
#include "bn_regular_bg_items_frame_0017.h"
#include "bn_regular_bg_items_frame_0018.h"
#include "bn_regular_bg_items_frame_0019.h"
#include "bn_regular_bg_items_frame_0020.h"
#include "bn_regular_bg_items_frame_0021.h"
#include "bn_regular_bg_items_frame_0022.h"
#include "bn_regular_bg_items_frame_0023.h"
#include "bn_regular_bg_items_frame_0024.h"
#include "bn_regular_bg_items_frame_0025.h"
#include "bn_regular_bg_items_frame_0026.h"
#include "bn_regular_bg_items_frame_0027.h"
#include "bn_regular_bg_items_frame_0028.h"
#include "bn_regular_bg_items_frame_0029.h"
#include "bn_regular_bg_items_frame_0030.h"
#include "bn_regular_bg_items_frame_0031.h"
#include "bn_regular_bg_items_frame_0032.h"
#include "bn_regular_bg_items_frame_0033.h"
#include "bn_regular_bg_items_frame_0034.h"
#include "bn_regular_bg_items_frame_0035.h"
#include "bn_regular_bg_items_frame_0036.h"
#include "bn_regular_bg_items_frame_0037.h"
#include "bn_regular_bg_items_frame_0038.h"
#include "bn_regular_bg_items_frame_0039.h"
#include "bn_regular_bg_items_frame_0040.h"
#include "bn_regular_bg_items_frame_0041.h"
#include "bn_regular_bg_items_frame_0042.h"
#include "bn_regular_bg_items_frame_0043.h"
#include "bn_regular_bg_items_frame_0044.h"
#include "bn_regular_bg_items_frame_0045.h"
#include "bn_regular_bg_items_frame_0046.h"
#include "bn_regular_bg_items_frame_0047.h"
#include "bn_regular_bg_items_frame_0048.h"
#include "bn_regular_bg_items_frame_0049.h"
#include "bn_regular_bg_items_frame_0050.h"
#include "bn_regular_bg_items_frame_0051.h"
#include "bn_regular_bg_items_frame_0052.h"
#include "bn_regular_bg_items_frame_0053.h"
#include "bn_regular_bg_items_frame_0054.h"
#include "bn_regular_bg_items_frame_0055.h"
#include "bn_regular_bg_items_frame_0056.h"
#include "bn_regular_bg_items_frame_0057.h"
#include "bn_regular_bg_items_frame_0058.h"
#include "bn_regular_bg_items_frame_0059.h"
#include "bn_regular_bg_items_frame_0060.h"
#include "bn_regular_bg_items_frame_0061.h"
#include "bn_regular_bg_items_frame_0062.h"
#include "bn_regular_bg_items_frame_0063.h"
#include "bn_regular_bg_items_frame_0064.h"
#include "bn_regular_bg_items_frame_0065.h"
#include "bn_regular_bg_items_frame_0066.h"
#include "bn_regular_bg_items_frame_0067.h"
#include "bn_regular_bg_items_frame_0068.h"
#include "bn_regular_bg_items_frame_0069.h"
#include "bn_regular_bg_items_frame_0070.h"
#include "bn_regular_bg_items_frame_0071.h"
#include "bn_regular_bg_items_frame_0072.h"
#include "bn_regular_bg_items_frame_0073.h"
#include "bn_regular_bg_items_frame_0074.h"
#include "bn_regular_bg_items_frame_0075.h"

namespace
{
    // Video intro configuration
    constexpr int VIDEO_FPS = 15;
    constexpr int VIDEO_FRAME_DELAY = 60 / VIDEO_FPS;
    constexpr int TOTAL_VIDEO_FRAMES = 75;

    // Page states
    enum class page_state
    {
        VIDEO_INTRO,
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

    namespace video
    {
        constexpr const bn::regular_bg_item* frames[] = {
            &bn::regular_bg_items::frame_0001,
            &bn::regular_bg_items::frame_0002,
            &bn::regular_bg_items::frame_0003,
            &bn::regular_bg_items::frame_0004,
            &bn::regular_bg_items::frame_0005,
            &bn::regular_bg_items::frame_0006,
            &bn::regular_bg_items::frame_0007,
            &bn::regular_bg_items::frame_0008,
            &bn::regular_bg_items::frame_0009,
            &bn::regular_bg_items::frame_0010,
            &bn::regular_bg_items::frame_0011,
            &bn::regular_bg_items::frame_0012,
            &bn::regular_bg_items::frame_0013,
            &bn::regular_bg_items::frame_0014,
            &bn::regular_bg_items::frame_0015,
            &bn::regular_bg_items::frame_0016,
            &bn::regular_bg_items::frame_0017,
            &bn::regular_bg_items::frame_0018,
            &bn::regular_bg_items::frame_0019,
            &bn::regular_bg_items::frame_0020,
            &bn::regular_bg_items::frame_0021,
            &bn::regular_bg_items::frame_0022,
            &bn::regular_bg_items::frame_0023,
            &bn::regular_bg_items::frame_0024,
            &bn::regular_bg_items::frame_0025,
            &bn::regular_bg_items::frame_0026,
            &bn::regular_bg_items::frame_0027,
            &bn::regular_bg_items::frame_0028,
            &bn::regular_bg_items::frame_0029,
            &bn::regular_bg_items::frame_0030,
            &bn::regular_bg_items::frame_0031,
            &bn::regular_bg_items::frame_0032,
            &bn::regular_bg_items::frame_0033,
            &bn::regular_bg_items::frame_0034,
            &bn::regular_bg_items::frame_0035,
            &bn::regular_bg_items::frame_0036,
            &bn::regular_bg_items::frame_0037,
            &bn::regular_bg_items::frame_0038,
            &bn::regular_bg_items::frame_0039,
            &bn::regular_bg_items::frame_0040,
            &bn::regular_bg_items::frame_0041,
            &bn::regular_bg_items::frame_0042,
            &bn::regular_bg_items::frame_0043,
            &bn::regular_bg_items::frame_0044,
            &bn::regular_bg_items::frame_0045,
            &bn::regular_bg_items::frame_0046,
            &bn::regular_bg_items::frame_0047,
            &bn::regular_bg_items::frame_0048,
            &bn::regular_bg_items::frame_0049,
            &bn::regular_bg_items::frame_0050,
            &bn::regular_bg_items::frame_0051,
            &bn::regular_bg_items::frame_0052,
            &bn::regular_bg_items::frame_0053,
            &bn::regular_bg_items::frame_0054,
            &bn::regular_bg_items::frame_0055,
            &bn::regular_bg_items::frame_0056,
            &bn::regular_bg_items::frame_0057,
            &bn::regular_bg_items::frame_0058,
            &bn::regular_bg_items::frame_0059,
            &bn::regular_bg_items::frame_0060,
            &bn::regular_bg_items::frame_0061,
            &bn::regular_bg_items::frame_0062,
            &bn::regular_bg_items::frame_0063,
            &bn::regular_bg_items::frame_0064,
            &bn::regular_bg_items::frame_0065,
            &bn::regular_bg_items::frame_0066,
            &bn::regular_bg_items::frame_0067,
            &bn::regular_bg_items::frame_0068,
            &bn::regular_bg_items::frame_0069,
            &bn::regular_bg_items::frame_0070,
            &bn::regular_bg_items::frame_0071,
            &bn::regular_bg_items::frame_0072,
            &bn::regular_bg_items::frame_0073,
            &bn::regular_bg_items::frame_0074,
            &bn::regular_bg_items::frame_0075
        };
        
        constexpr int frame_count = sizeof(frames) / sizeof(frames[0]);
    }

    class resume_game
    {
    private:
        bn::sprite_text_generator _text_generator;
        bn::sprite_text_generator _small_text_generator;
        bn::vector<bn::sprite_ptr, 128> _text_sprites;
        bn::vector<bn::sprite_ptr, 64> _small_text_sprites;
        page_state _current_state;
        carousel_page _current_carousel_page;
        experience_section _selected_experience;
        int _highlight_index;
        experience_section _detail_section;
        
        // Video intro variables
        bn::optional<bn::regular_bg_ptr> _video_bg;
        int _video_frame_index;
        int _video_frame_counter;
        bool _video_audio_started;
        
        // Main background variable
        bn::optional<bn::regular_bg_ptr> _main_background;
        
        // Arrow sprites for carousel navigation
        bn::optional<bn::sprite_ptr> _left_arrow;
        bn::optional<bn::sprite_ptr> _right_arrow;
        int _pulse_counter;

    public:
        resume_game() :
            _text_generator(common::variable_8x16_sprite_font),
            _small_text_generator(common::variable_8x8_sprite_font),
            _current_state(page_state::VIDEO_INTRO),
            _current_carousel_page(carousel_page::EXPERIENCE),
            _selected_experience(experience_section::HAIVISION),
            _highlight_index(0),
            _video_frame_index(0),
            _video_frame_counter(0),
            _video_audio_started(false),
            _pulse_counter(0)
        {
            // Start with video intro
            show_video_frame(0);
            
            // Start the intro audio
            bn::sound_items::intro_audio.play();
            _video_audio_started = true;
        }

        void update()
        {
            switch (_current_state)
            {
                case page_state::VIDEO_INTRO:
                    update_video_intro();
                    break;
                    
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
        void show_video_frame(int frame_index)
        {
            _video_bg.reset();
            
            if (frame_index < video::frame_count)
            {
                _video_bg = video::frames[frame_index]->create_bg(0, 0);
            }
        }

        void update_video_intro()
        {
            // Allow skipping with any button
            if (bn::keypad::any_pressed())
            {
                end_video_intro();
                return;
            }

            // Update frame counter
            _video_frame_counter++;
            
            // Check if it's time for next frame
            if (_video_frame_counter >= VIDEO_FRAME_DELAY)
            {
                _video_frame_counter = 0;
                _video_frame_index++;
                
                // Check if video is complete
                if (_video_frame_index >= TOTAL_VIDEO_FRAMES)
                {
                    end_video_intro();
                }
                else
                {
                    show_video_frame(_video_frame_index);
                }
            }
        }

        void end_video_intro()
        {
            // Clear video background
            _video_bg.reset();
            
            // Stop intro audio if it's still playing
            if (_video_audio_started)
            {
                bn::sound::stop_all();
                _video_audio_started = false;
            }
            
            // Set up normal background and transition to intro
            setup_background();
            bn::music_items::cyberrid.play(0.5, true);
            _current_state = page_state::INTRO;
            show_intro_page();
        }

        void setup_background()
        {
            // Clear any existing background first
            _main_background.reset();
            
            // Create the background from your converted image
            _main_background = bn::regular_bg_items::main_background.create_bg(0, 0);
            
            // Set priority so it appears behind sprites
            _main_background->set_priority(3);
            
            // Set a fallback color
            bn::bg_palettes::set_transparent_color(bn::color(2, 4, 8));  // Dark blue
        }

        void clear_background()
        {
            _main_background.reset();
        }

        void clear_text()
        {
            _text_sprites.clear();
            _small_text_sprites.clear();
        }
        
        void create_navigation_arrows()
        {
            // Create left arrow
            _left_arrow = bn::sprite_items::arrow_left.create_sprite(-110, 0);
            
            // Create right arrow  
            _right_arrow = bn::sprite_items::arrow_right.create_sprite(110, 0);
            
            // Reset pulse counter for smooth animation
            _pulse_counter = 0;
        }
        
        void clear_navigation_arrows()
        {
            _left_arrow.reset();
            _right_arrow.reset();
        }
        
        void update_arrow_pulse()
        {
            _pulse_counter++;
            
            // Move arrows in and out
            int cycle_length = 60;
            int half_cycle = cycle_length / 2;
            int position = _pulse_counter % cycle_length;
            
            bn::fixed offset;
            if (position < half_cycle)
            {
                offset = bn::fixed(5) * position / half_cycle;
            }
            else
            {
                offset = bn::fixed(5) - (bn::fixed(5) * (position - half_cycle) / half_cycle);
            }
            
            if (_left_arrow)
            {
                _left_arrow->set_x(-110 + offset);  // Move right then left
            }
            
            if (_right_arrow)
            {
                _right_arrow->set_x(110 - offset);   // Move left then right
            }
        }

        void show_intro_page()
        {
            clear_text();
            
            _text_generator.set_center_alignment();
            
            // Use large font for main title
            _text_generator.generate(0, -60, "SAMUEL CALVERT", _text_sprites);
            
            // Use small font for subtitle and details
            _small_text_generator.set_center_alignment();
            _small_text_generator.generate(0, -40, "computer engineering", _small_text_sprites);
            _small_text_generator.generate(0, -30, "professional", _small_text_sprites);
            
            _small_text_generator.generate(0, -10, "contact:", _small_text_sprites);
            _small_text_generator.generate(0, 0, "(302) 513-3155", _small_text_sprites);
            _small_text_generator.generate(0, 10, "hello@samuelcalvert.com", _small_text_sprites);
            
            // Use large font for action prompt
            _text_generator.generate(0, 50, "Press START", _text_sprites);
        }

        void show_carousel_page()
        {
            clear_text();
            
            // Show navigation hints
            _text_generator.set_center_alignment();
            _text_generator.generate(0, -70, get_page_title(), _text_sprites);
            
            // Use small font for navigation instructions
            // _small_text_generator.set_center_alignment();
            // _small_text_generator.generate(0, 65, "L/R: Navigate | U/D: Select | A: OK", _small_text_sprites);
            
            // Show page content
            switch (_current_carousel_page)
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
            switch (_current_carousel_page)
            {
                case carousel_page::EXPERIENCE: 
                    return "EXPERIENCE";
                    
                case carousel_page::PROJECTS: 
                    return "PROJECTS";
                    
                case carousel_page::SKILLS: 
                    return "SKILLS";
                    
                case carousel_page::EDUCATION: 
                    return "EDUCATION";
                    
                default: 
                    return "";
            }
        }

        void show_experience_page()
        {
            _text_generator.set_left_alignment();
            _small_text_generator.set_left_alignment();
            int y_pos = -40;
            
            // Haivision - use large font for company names
            if (_highlight_index == 0)
            {
                _text_generator.generate(-100, y_pos, "> Haivision", _text_sprites);
            }
            else
            {
                _text_generator.generate(-100, y_pos, "  Haivision", _text_sprites);
            }
            // Use small font for job titles
            _small_text_generator.generate(-90, y_pos + 15, "tech support engineer", _small_text_sprites);
            
            y_pos += 35;
            
            // Lab Staff
            if (_highlight_index == 1)
            {
                _text_generator.generate(-100, y_pos, "> UD EECIS Dept", _text_sprites);
            }
            else
            {
                _text_generator.generate(-100, y_pos, "  UD EECIS Dept", _text_sprites);
            }
            _small_text_generator.generate(-90, y_pos + 15, "lab staff", _small_text_sprites);
            
            y_pos += 35;
            
            // ML Research
            if (_highlight_index == 2)
            {
                _text_generator.generate(-100, y_pos, "> UD Machine Learn", _text_sprites);
            }
            else
            {
                _text_generator.generate(-100, y_pos, "  UD Machine Learn", _text_sprites);
            }
            _small_text_generator.generate(-90, y_pos + 15, "research intern", _small_text_sprites);
        }

        void show_projects_page()
        {
            _text_generator.set_left_alignment();
            _small_text_generator.set_left_alignment();
            
            // Use large font for project title
            _text_generator.generate(-100, -35, "Homelab K8s Infra", _text_sprites);
            
            // Use small font for details
            _small_text_generator.generate(-90, -15, "- multi-environment clusters", _small_text_sprites);
            _small_text_generator.generate(-90, -5, "- infrastructure as code", _small_text_sprites);
            _small_text_generator.generate(-90, 5, "- gitops deployment methods", _small_text_sprites);
            _small_text_generator.generate(-90, 15, "- prometheus monitoring", _small_text_sprites);
            _small_text_generator.generate(-90, 25, "- automated ci/cd pipelines", _small_text_sprites);
            _small_text_generator.generate(-90, 35, "- service mesh architecture", _small_text_sprites);
        }

        void show_skills_page()
        {
            _text_generator.set_left_alignment();
            _small_text_generator.set_left_alignment();
            
            // Use large font for categories
            _text_generator.generate(-100, -45, "Infrastructure:", _text_sprites);
            // Use small font for skills
            _small_text_generator.generate(-90, -30, "kubernetes, docker, terraform", _small_text_sprites);
            _small_text_generator.generate(-90, -20, "ansible, helm, fluxcd", _small_text_sprites);
            
            _text_generator.generate(-100, -5, "Programming:", _text_sprites);
            _small_text_generator.generate(-90, 10, "bash, python, go", _small_text_sprites);
            
            _text_generator.generate(-100, 25, "Networking:", _text_sprites);
            _small_text_generator.generate(-90, 40, "wireshark, nmap, GNS3", _small_text_sprites);
        }

        void show_education_page()
        {
            _text_generator.set_center_alignment();
            _small_text_generator.set_center_alignment();
            
            // Use large font for university name
            _text_generator.generate(0, -25, "University of Delaware", _text_sprites);
            
            // Use small font for degree details
            _small_text_generator.generate(0, -5, "bachelor of science", _small_text_sprites);
            _small_text_generator.generate(0, 5, "computer engineering", _small_text_sprites);
            _small_text_generator.generate(0, 15, "graduated may 2022", _small_text_sprites);
            
            // Additional details with small font
            //_small_text_generator.generate(0, 35, "GPA: 3.5/4.0", _small_text_sprites);
            //_small_text_generator.generate(0, 45, "Dean's List: Fall 2020, Spring 2021", _small_text_sprites);
        }

        void show_experience_detail()
        {
            clear_text();
            _text_generator.set_center_alignment();
            _small_text_generator.set_left_alignment();
            
            switch (_detail_section)
            {
                case experience_section::HAIVISION:
                    // Use large font for company name
                    _text_generator.generate(0, -65, "HAIVISION", _text_sprites);
                    
                    // Use small font for date and details
                    _small_text_generator.set_center_alignment();
                    _small_text_generator.generate(0, -50, "July 2022 - Present", _small_text_sprites);
                    _small_text_generator.set_left_alignment();
                    _small_text_generator.generate(-110, -30, "- deploy & troubleshoot solutions", _small_text_sprites);
                    _small_text_generator.generate(-110, -20, "- build custom packer images", _small_text_sprites);
                    _small_text_generator.generate(-110, -10, "- Manage 500+ endpoints globally", _small_text_sprites);
                    _small_text_generator.generate(-110, 0, "- optimize stream performance", _small_text_sprites);
                    _small_text_generator.generate(-110, 10, "- handle 50Gbps peak traffic", _small_text_sprites);
                    _small_text_generator.generate(-110, 20, "- AWS cloud deployments", _small_text_sprites);
                    _small_text_generator.generate(-110, 30, "- python automation scripts", _small_text_sprites);
                    break;
                    
                case experience_section::LAB_STAFF:
                    _text_generator.generate(0, -65, "LAB STAFF", _text_sprites);
                    _small_text_generator.set_center_alignment();
                    _small_text_generator.generate(0, -50, "June 2019 - August 2021", _small_text_sprites);
                    _small_text_generator.set_left_alignment();
                    _small_text_generator.generate(-110, -30, "- maintain 25 production servers", _small_text_sprites);
                    _small_text_generator.generate(-110, -20, "- VMware ESXi virtualization", _small_text_sprites);
                    _small_text_generator.generate(-110, -10, "- SLURM job scheduling system", _small_text_sprites);
                    _small_text_generator.generate(-110, 0, "- ansible configuration management", _small_text_sprites);
                    _small_text_generator.generate(-110, 10, "- manage 100TB+ datasets", _small_text_sprites);
                    _small_text_generator.generate(-110, 20, "- network security hardening", _small_text_sprites);
                    _small_text_generator.generate(-110, 30, "- user support & training", _small_text_sprites);
                    break;
                    
                case experience_section::MACHINE_LEARNING:
                    _text_generator.generate(0, -65, "ML RESEARCH", _text_sprites);
                    _small_text_generator.set_center_alignment();
                    _small_text_generator.generate(0, -50, "June 2020 - August 2020", _small_text_sprites);
                    _small_text_generator.set_left_alignment();
                    _small_text_generator.generate(-110, -30, "- network intrusion detection", _small_text_sprites);
                    _small_text_generator.generate(-110, -20, "- GAN neural networks design", _small_text_sprites);
                    _small_text_generator.generate(-110, -10, "- GNS3 virtual network labs", _small_text_sprites);
                    _small_text_generator.generate(-110, 0, "- docker containerization", _small_text_sprites);
                    _small_text_generator.generate(-110, 10, "- research documentation", _small_text_sprites);
                    _small_text_generator.generate(-110, 20, "- tensorflow implementation", _small_text_sprites);
                    _small_text_generator.generate(-110, 30, "- data pipeline development", _small_text_sprites);
                    break;
                    
                default:
                    break;
            }
            
            // Use small font for back instruction
            _small_text_generator.set_center_alignment();
            _small_text_generator.generate(0, 55, "Press B to go back", _small_text_sprites);
        }

        void update_intro()
        {
            if (bn::keypad::start_pressed() || bn::keypad::a_pressed())
            {
                bn::sound_items::alert.play();
                _current_state = page_state::MAIN_CAROUSEL;
                _highlight_index = 0;
                create_navigation_arrows();  // Create arrows once when entering carousel
                show_carousel_page();
            }
        }

        void update_carousel()
        {
            // Update arrow pulsing animation
            update_arrow_pulse();
            
            // Handle B button to go back to intro
            if (bn::keypad::b_pressed())
            {
                bn::sound_items::alert.play();
                clear_navigation_arrows();
                _current_state = page_state::INTRO;
                show_intro_page();
                return;
            }

            // Navigation between pages
            if (bn::keypad::left_pressed())
            {
                bn::sound_items::alert.play();
                int page_index = static_cast<int>(_current_carousel_page);
                page_index = (page_index - 1 + 4) % 4;
                _current_carousel_page = static_cast<carousel_page>(page_index);
                _highlight_index = 0;
                show_carousel_page();
            }
            else if (bn::keypad::right_pressed())
            {
                bn::sound_items::alert.play();
                int page_index = static_cast<int>(_current_carousel_page);
                page_index = (page_index + 1) % 4;
                _current_carousel_page = static_cast<carousel_page>(page_index);
                _highlight_index = 0;
                show_carousel_page();
            }
            
            // Handle experience page selection
            if (_current_carousel_page == carousel_page::EXPERIENCE)
            {
                if (bn::keypad::up_pressed())
                {
                    bn::sound_items::alert.play();
                    _highlight_index = (_highlight_index - 1 + 3) % 3;
                    show_carousel_page();
                }
                else if (bn::keypad::down_pressed())
                {
                    bn::sound_items::alert.play();
                    _highlight_index = (_highlight_index + 1) % 3;
                    show_carousel_page();
                }
                
                if (bn::keypad::a_pressed())
                {
                    bn::sound_items::alert.play();
                    clear_navigation_arrows();
                    _current_state = page_state::EXPERIENCE_DETAIL;
                    _detail_section = static_cast<experience_section>(_highlight_index);
                    show_experience_detail();
                }
            }
        }

        void update_detail()
        {
            if (bn::keypad::b_pressed())
            {
                bn::sound_items::alert.play();
                _current_state = page_state::MAIN_CAROUSEL;
                create_navigation_arrows();  // Recreate arrows when returning to carousel
                show_carousel_page();
            }
        }
    };
}

int main()
{
    bn::core::init();
    
    resume_game game;
    
    while (true)
    {
        game.update();
        bn::core::update();
    }
}