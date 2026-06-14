#include "SimConfig.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <cctype>

namespace
{
std::string trim(const std::string& s)
{
    const char* ws = " \t\r\n";
    const size_t a = s.find_first_not_of(ws);
    if (a == std::string::npos) return "";
    const size_t b = s.find_last_not_of(ws);
    return s.substr(a, b - a + 1);
}

// Convierte toda la cadena a float; falla si sobra texto no numérico ("12abc").
bool toFloat(const std::string& s, float& out)
{
    try
    {
        size_t pos = 0;
        const float v = std::stof(s, &pos);
        while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos]))) ++pos;
        if (pos != s.size()) return false;
        out = v;
        return true;
    }
    catch (...) { return false; }
}
} // namespace

SimConfig parseConfig(std::istream& in, SimConfig base)
{
    auto setF = [](float& field, const std::string& value, const std::string& key)
    {
        float f;
        if (toFloat(value, f)) field = f;
        else std::cerr << "[SimConfig] valor inválido para '" << key << "': '" << value << "'\n";
    };
    auto setI = [](int& field, const std::string& value, const std::string& key)
    {
        float f;
        if (toFloat(value, f)) field = static_cast<int>(f);
        else std::cerr << "[SimConfig] valor inválido para '" << key << "': '" << value << "'\n";
    };

    std::string line;
    while (std::getline(in, line))
    {
        const size_t hash = line.find('#'); // comentario hasta fin de línea
        if (hash != std::string::npos) line.erase(hash);

        line = trim(line);
        if (line.empty()) continue;

        const size_t eq = line.find('=');
        if (eq == std::string::npos)
        {
            std::cerr << "[SimConfig] línea sin '=': '" << line << "'\n";
            continue;
        }

        const std::string key = trim(line.substr(0, eq));
        const std::string value = trim(line.substr(eq + 1));
        if (key.empty() || value.empty())
        {
            std::cerr << "[SimConfig] línea malformada: '" << line << "'\n";
            continue;
        }

        // Solver
        if      (key == "restDensity")      setF(base.restDensity, value, key);
        else if (key == "gasConstant")      setF(base.gasConstant, value, key);
        else if (key == "viscosity")        setF(base.viscosity, value, key);
        else if (key == "smoothingLength")  setF(base.smoothingLength, value, key);
        else if (key == "surfaceTension")   setF(base.surfaceTension, value, key);
        else if (key == "surfaceThreshold") setF(base.surfaceThreshold, value, key);
        // Escena
        else if (key == "numParticles")     setI(base.numParticles, value, key);
        else if (key == "spacing")          setF(base.spacing, value, key);
        else if (key == "mass")             setF(base.mass, value, key);
        else if (key == "originX")          setF(base.originX, value, key);
        else if (key == "originY")          setF(base.originY, value, key);
        // Integración
        else if (key == "dt")               setF(base.dt, value, key);
        else if (key == "substeps")         setI(base.substeps, value, key);
        // Mundo
        else if (key == "gravity")          setF(base.gravity, value, key);
        else if (key == "restitution")      setF(base.restitution, value, key);
        else if (key == "domainMinX")       setF(base.domainMinX, value, key);
        else if (key == "domainMinY")       setF(base.domainMinY, value, key);
        else if (key == "domainMaxX")       setF(base.domainMaxX, value, key);
        else if (key == "domainMaxY")       setF(base.domainMaxY, value, key);
        // Render
        else if (key == "pointSize")        setF(base.pointSize, value, key);
        else std::cerr << "[SimConfig] clave desconocida: '" << key << "'\n";
    }

    return base;
}

SimConfig loadConfig(const std::string& path)
{
    std::ifstream file(path);
    if (!file)
    {
        std::cerr << "[SimConfig] no se pudo abrir '" << path << "'; usando defaults\n";
        return SimConfig{};
    }
    return parseConfig(file, SimConfig{});
}
