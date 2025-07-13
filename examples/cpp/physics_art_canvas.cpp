#include <fern/fern.hpp>
#include <memory>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <string>
#include <ctime>

using namespace Fern;

// Physics Art Canvas - Paint with particles that have realistic physics!
struct Particle {
    float x, y;           // Position
    float vx, vy;         // Velocity
    float life = 1.0f;    // Life remaining (0-1)
    float size = 3.0f;    // Particle size
    uint32_t color;       // Color
    bool sticky = false;  // Does it stick to surfaces?
    bool bouncy = true;   // Does it bounce?
    float gravity = 9.8f; // Personal gravity
    
    Particle(float px, float py, float pvx, float pvy, uint32_t c) 
        : x(px), y(py), vx(pvx), vy(pvy), color(c) {}
};

struct Attractor {
    float x, y;
    float strength;
    float radius;
    uint32_t color;
    bool repel; // Attract or repel?
    
    Attractor(float px, float py, float s, float r, bool rep = false) 
        : x(px), y(py), strength(s), radius(r), repel(rep) {
        color = repel ? Colors::Red : Colors::Cyan;
    }
};

struct Trail {
    std::vector<Point> points;
    uint32_t color;
    float width;
    float life;
    
    Trail(uint32_t c, float w) : color(c), width(w), life(1.0f) {}
};

// Art modes
enum class ArtMode {
    ParticlePaint,    // Paint with physics particles
    Fireworks,        // Create firework explosions
    FluidSim,         // Fluid-like behavior
    Lightning,        // Electric arcs
    Galaxy,           // Stellar formation
    Magnet,           // Magnetic field visualization
    Quantum           // Quantum wave effects
};

// Global state
std::vector<Particle> particles;
std::vector<Attractor> attractors;
std::vector<Trail> trails;
ArtMode currentMode = ArtMode::ParticlePaint;
float windX = 0.0f, windY = 0.0f;
float gravity = 50.0f;
float friction = 0.98f;
bool paused = false;
std::mt19937 rng(std::time(nullptr));

// UI elements (minimal, focused on art)
std::shared_ptr<ButtonWidget> modeButton;
std::shared_ptr<SliderWidget> gravitySlider;
std::shared_ptr<ButtonWidget> clearButton;
std::shared_ptr<TextWidget> modeText;

// Colors for different modes
const std::vector<uint32_t> modeColors = {
    0xFF00FFFF,  // Cyan for ParticlePaint
    0xFFFFD700,  // Gold for Fireworks
    0xFF4169E1,  // Blue for FluidSim
    0xFFFFFFFF,  // White for Lightning
    0xFF9932CC,  // Purple for Galaxy
    0xFFFF4500,  // Red for Magnet
    0xFF00FF00   // Green for Quantum
};

const std::vector<std::string> modeNames = {
    "Particle Paint", "Fireworks", "Fluid Sim", "Lightning", 
    "Galaxy", "Magnet", "Quantum"
};

uint32_t getCurrentColor() {
    return modeColors[static_cast<int>(currentMode)];
}

void updateParticles(float deltaTime) {
    // Update all particles
    for (auto it = particles.begin(); it != particles.end();) {
        auto& p = *it;
        
        // Apply gravity
        p.vy += gravity * deltaTime;
        
        // Apply wind
        p.vx += windX * deltaTime;
        p.vy += windY * deltaTime;
        
        // Mode-specific physics
        switch (currentMode) {
            case ArtMode::FluidSim: {
                // Fluid-like viscosity
                p.vx *= 0.95f;
                p.vy *= 0.95f;
                
                // Attract to nearby particles (cohesion)
                for (const auto& other : particles) {
                    if (&other == &p) continue;
                    float dx = other.x - p.x;
                    float dy = other.y - p.y;
                    float dist = std::sqrt(dx*dx + dy*dy);
                    if (dist < 50.0f && dist > 0.1f) {
                        float force = 10.0f / (dist * dist);
                        p.vx += force * dx * deltaTime;
                        p.vy += force * dy * deltaTime;
                    }
                }
                break;
            }
            
            case ArtMode::Galaxy: {
                // Orbital motion around center
                float centerX = 400.0f;
                float centerY = 300.0f;
                float dx = centerX - p.x;
                float dy = centerY - p.y;
                float dist = std::sqrt(dx*dx + dy*dy);
                if (dist > 1.0f) {
                    float orbitalForce = 5000.0f / (dist * dist);
                    p.vx += orbitalForce * dx / dist * deltaTime;
                    p.vy += orbitalForce * dy / dist * deltaTime;
                    
                    // Add tangential velocity for spiral
                    p.vx += -dy / dist * 20.0f * deltaTime;
                    p.vy += dx / dist * 20.0f * deltaTime;
                }
                break;
            }
            
            case ArtMode::Quantum: {
                // Wave-like interference patterns
                float waveTime = p.life * 10.0f;
                p.x += 5.0f * std::sin(waveTime) * deltaTime;
                p.y += 5.0f * std::cos(waveTime * 1.3f) * deltaTime;
                break;
            }
            
            case ArtMode::Magnet: {
                // Magnetic field effects from attractors
                for (const auto& attractor : attractors) {
                    float dx = attractor.x - p.x;
                    float dy = attractor.y - p.y;
                    float dist = std::sqrt(dx*dx + dy*dy);
                    if (dist < attractor.radius && dist > 1.0f) {
                        float force = attractor.strength / (dist * dist);
                        if (attractor.repel) force = -force;
                        
                        // Magnetic fields create perpendicular forces
                        p.vx += force * (-dy) / dist * deltaTime;
                        p.vy += force * dx / dist * deltaTime;
                    }
                }
                break;
            }
            
            default:
                break;
        }
        
        // Apply friction
        p.vx *= friction;
        p.vy *= friction;
        
        // Update position
        p.x += p.vx * deltaTime;
        p.y += p.vy * deltaTime;
        
        // Bounce off walls
        if (p.bouncy) {
            if (p.x < 0 || p.x > 800) {
                p.vx = -p.vx * 0.8f;
                p.x = std::max(0.0f, std::min(800.0f, p.x));
            }
            if (p.y < 0 || p.y > 600) {
                p.vy = -p.vy * 0.8f;
                p.y = std::max(0.0f, std::min(600.0f, p.y));
            }
        }
        
        // Age particles
        p.life -= deltaTime * 0.2f;
        
        // Fade color based on life
        uint8_t alpha = static_cast<uint8_t>(p.life * 255);
        p.color = (p.color & 0x00FFFFFF) | (alpha << 24);
        
        // Remove dead particles
        if (p.life <= 0.0f) {
            it = particles.erase(it);
        } else {
            ++it;
        }
    }
    
    // Update trails
    for (auto it = trails.begin(); it != trails.end();) {
        it->life -= deltaTime * 0.5f;
        if (it->life <= 0.0f) {
            it = trails.erase(it);
        } else {
            ++it;
        }
    }
}

void createParticles(float x, float y, int count = 10) {
    for (int i = 0; i < count; i++) {
        float angle = (rng() % 360) * M_PI / 180.0f;
        float speed = 50.0f + (rng() % 100);
        float vx = std::cos(angle) * speed;
        float vy = std::sin(angle) * speed;
        
        uint32_t color = getCurrentColor();
        
        // Mode-specific particle creation
        switch (currentMode) {
            case ArtMode::Fireworks: {
                // Explosive radial pattern
                speed = 100.0f + (rng() % 200);
                vx = std::cos(angle) * speed;
                vy = std::sin(angle) * speed;
                // Add some upward bias
                vy -= 50.0f;
                
                // Random firework colors
                std::vector<uint32_t> fireworkColors = {
                    Colors::Red, Colors::Blue, Colors::Green, Colors::Yellow,
                    Colors::Purple, Colors::Orange, Colors::White, Colors::Pink
                };
                color = fireworkColors[rng() % fireworkColors.size()];
                break;
            }
            
            case ArtMode::Lightning: {
                // Create branching lightning
                count = 1; // One particle at a time for lightning
                vx = (rng() % 200) - 100;
                vy = (rng() % 100) - 50;
                color = Colors::White;
                break;
            }
            
            case ArtMode::Galaxy: {
                // Slower, more massive particles
                speed = 20.0f + (rng() % 50);
                vx = std::cos(angle) * speed;
                vy = std::sin(angle) * speed;
                color = (rng() % 2) ? Colors::Purple : Colors::Blue;
                break;
            }
            
            default:
                break;
        }
        
        Particle p(x, y, vx, vy, color);
        p.size = 2.0f + (rng() % 4);
        
        if (currentMode == ArtMode::FluidSim) {
            p.sticky = true;
            p.gravity = 20.0f;
        }
        
        particles.push_back(p);
    }
}

void createSpecialEffects(float x, float y) {
    switch (currentMode) {
        case ArtMode::Lightning: {
            // Create lightning bolt trail
            Trail lightning(Colors::White, 3.0f);
            
            // Generate zigzag pattern
            float currentX = x;
            float currentY = y;
            lightning.points.push_back(Point(currentX, currentY));
            
            for (int i = 0; i < 20; i++) {
                currentX += (rng() % 40) - 20;
                currentY += (rng() % 30) + 10;
                lightning.points.push_back(Point(currentX, currentY));
                
                if (currentY > 600) break;
            }
            
            trails.push_back(lightning);
            break;
        }
        
        case ArtMode::Magnet: {
            // Create magnetic attractor/repeller
            bool repel = (rng() % 2) == 0;
            attractors.emplace_back(x, y, 100.0f, 80.0f, repel);
            break;
        }
        
        default:
            break;
    }
}

void drawParticles() {
    // Draw trails first (background)
    for (const auto& trail : trails) {
        if (trail.points.size() < 2) continue;
        
        uint8_t alpha = static_cast<uint8_t>(trail.life * 255);
        uint32_t trailColor = (trail.color & 0x00FFFFFF) | (alpha << 24);
        
        for (size_t i = 1; i < trail.points.size(); i++) {
            Draw::line(trail.points[i-1].x, trail.points[i-1].y,
                      trail.points[i].x, trail.points[i].y, 1, trailColor);
        }
    }
    
    // Draw attractors
    for (const auto& attractor : attractors) {
        uint32_t color = attractor.repel ? Colors::Red : Colors::Cyan;
        Draw::circle(attractor.x, attractor.y, attractor.radius, color & 0x40FFFFFF); // Semi-transparent
        Draw::circle(attractor.x, attractor.y, 5, color);
    }
    
    // Draw particles
    for (const auto& particle : particles) {
        if (currentMode == ArtMode::Lightning) {
            // Lightning particles glow
            Draw::circle(particle.x, particle.y, particle.size + 2, Colors::White & 0x40FFFFFF);
        }
        
        Draw::circle(particle.x, particle.y, particle.size, particle.color);
        
        if (currentMode == ArtMode::Quantum) {
            // Quantum interference patterns
            float wave = std::sin(particle.life * 20.0f) * 0.5f + 0.5f;
            uint32_t waveColor = (particle.color & 0x00FFFFFF) | (static_cast<uint8_t>(wave * 128) << 24);
            Draw::circle(particle.x, particle.y, particle.size * 2, waveColor);
        }
    }
}

void initializeUI() {
    // Mode cycling button
    auto modeConfig = ButtonConfig{
        .x = 10,
        .y = 10,
        .width = 120,
        .height = 30,
        .normalColor = Colors::DarkGray,
        .hoverColor = Colors::Gray,
        .pressColor = Colors::LightGray,
        .label = "Mode: Paint",
        .textScale = 1,
        .textColor = Colors::White
    };
    modeButton = Button(modeConfig, true);
    
    // Gravity slider
    auto gravityConfig = SliderConfig(150, 10, 200, 25)
        .range(-100.0f, 200.0f)
        .initialValue(gravity)
        .style(SliderStyle()
            .trackColor(Colors::Gray)
            .fillColor(Colors::Green)
            .thumbColor(Colors::White)
            .showValue(true));
    gravitySlider = Slider(gravityConfig, true);
    
    // Clear button
    auto clearConfig = ButtonConfig{
        .x = 370,
        .y = 10,
        .width = 80,
        .height = 30,
        .normalColor = Colors::Red,
        .hoverColor = Colors::LightRed,
        .pressColor = Colors::DarkRed,
        .label = "Clear",
        .textScale = 1,
        .textColor = Colors::White
    };
    clearButton = Button(clearConfig, true);
    
    // Mode description
    modeText = Text(Point(10, 50), "Click and drag to paint with physics particles!", 1, Colors::White, true);
    
    // Connect events
    modeButton->onClick.connect([]() {
        int nextMode = (static_cast<int>(currentMode) + 1) % 7; // 7 total modes
        currentMode = static_cast<ArtMode>(nextMode);
        // modeButton->setLabel("Mode: " + modeNames[nextMode]);
        
        // Update description
        std::string description;
        switch (currentMode) {
            case ArtMode::ParticlePaint: description = "Click and drag to paint with physics particles!"; break;
            case ArtMode::Fireworks: description = "Click to create firework explosions!"; break;
            case ArtMode::FluidSim: description = "Create fluid-like particle systems!"; break;
            case ArtMode::Lightning: description = "Generate electric lightning bolts!"; break;
            case ArtMode::Galaxy: description = "Form spiral galaxies with orbital physics!"; break;
            case ArtMode::Magnet: description = "Place magnetic attractors and repellers!"; break;
            case ArtMode::Quantum: description = "Paint with quantum wave interference!"; break;
        }
        modeText->setText(description);
        
        // Clear when switching modes for better visual clarity
        particles.clear();
        trails.clear();
        attractors.clear();
    });
    
    clearButton->onClick.connect([]() {
        particles.clear();
        trails.clear();
        attractors.clear();
    });
}

void update(float deltaTime) {
    if (paused) return;
    
    // Update gravity from slider
    gravity = gravitySlider->getValue();
    
    // Handle continuous input for painting
    const auto& input = Input::getState();
    
    if (input.mouseDown) {
        float mouseX = input.mouseX;
        float mouseY = input.mouseY;
        
        // Don't paint on UI
        if (mouseY > 80) {
            static float lastParticleTime = 0.0f;
            lastParticleTime += deltaTime;
            
            if (lastParticleTime > 0.05f) { // Create particles 20 times per second when painting
                int particleCount = 5;
                if (currentMode == ArtMode::Fireworks) particleCount = 20;
                if (currentMode == ArtMode::Galaxy) particleCount = 15;
                
                createParticles(mouseX, mouseY, particleCount);
                createSpecialEffects(mouseX, mouseY);
                lastParticleTime = 0.0f;
            }
        }
    }
    
    // Single click effects
    if (input.mouseClicked) {
        float mouseX = input.mouseX;
        float mouseY = input.mouseY;
        
        if (mouseY > 80) { // Don't interfere with UI
            if (currentMode == ArtMode::Fireworks) {
                createParticles(mouseX, mouseY, 50); // Big explosion
            }
        }
    }
    
    // Keyboard shortcuts
    if (input.keyPressed) {
        switch (input.lastKeyPressed) {
            case KeyCode::Space:
                paused = !paused;
                break;
            case KeyCode::C:
                particles.clear();
                trails.clear();
                attractors.clear();
                break;
            case KeyCode::R:
                // Random wind
                windX = (rng() % 200) - 100;
                windY = (rng() % 100) - 50;
                break;
            default:
                // Ignore other keys
                break;
        }
    }
    
    updateParticles(deltaTime);
    
    // Fade old attractors
    for (auto it = attractors.begin(); it != attractors.end();) {
        it->strength *= 0.999f;
        if (it->strength < 1.0f) {
            it = attractors.erase(it);
        } else {
            ++it;
        }
    }
}

void draw() {
    static float lastUpdateTime = 0.0f;
    static bool firstFrame = true;
    
    if (firstFrame) {
        lastUpdateTime = 0.0f;
        firstFrame = false;
    }
    
    float deltaTime = 0.016f; // 60 FPS
    
    // Background with slight fade for trails
    Draw::fill(0xFF000010); // Very dark with slight transparency effect
    
    // Update and draw
    update(deltaTime);
    drawParticles();
    
    // Draw UI info
    Text(Point(500, 15), "Particles: " + std::to_string(particles.size()), 1, Colors::White, true);
    Text(Point(500, 30), "Trails: " + std::to_string(trails.size()), 1, Colors::White, true);
    Text(Point(650, 15), "SPACE: Pause, C: Clear, R: Wind", 1, Colors::LightGray, true);
    Text(Point(650, 30), paused ? "PAUSED" : "RUNNING", 1, paused ? Colors::Red : Colors::Green, true);
}

int main() {
    // Initialize Fern
    Fern::initialize();
    
    // Setup UI
    initializeUI();
    
    // Set callbacks
    Fern::setDrawCallback(draw);
    
    // Start the render loop
    Fern::startRenderLoop();
    
    return 0;
}
