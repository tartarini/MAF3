
#include <{{ compoundname }}.h>

{% if namespace %}using namespace {{ namespace }}; {% endif %}

// {{ compoundname }} decorator
class {{compoundname}}Decorator : public QObject
{
  Q_OBJECT

public Q_SLOTS:
  {% for method in public_func %}
    {% if not method.operator and not method.destructor and not method.macro%}
        {% if method.name == compoundname %}
            {{ compoundname }}* new_{{ compoundname }}({% for p in method.param %} {{p.type}} {{p.declname}}{% if not loop.last%},{%endif%}{% endfor %}) {
                return new {{ compoundname }}({% for p in method.param %} {{ p.declname }}{% if not loop.last%},{%endif%} {% endfor %});
        }{% else %}
            {% if method.type %}{{ method.type }}{%else%}void{%endif%} {{ method.name}} ( {{compoundname}}* selfObj {% for p in method.param %}, {{p.type}} {{p.declname}}{% endfor %}) {
                {% if method.type and method.type != 'void'%}return{% endif %} selfObj->{{ method.name }}({% for p in method.param %} {{ p.declname }}{% if not loop.last%},{%endif%}{% endfor %});
        }{% endif %}
    {% endif %}
  {% endfor %}
  void delete_{{compoundname}}({{compoundname}}* selfObj) {
    {% if public_destructor %} delete selfObj;{% else %}selfObj->{{destructor_method}}();{%endif%}
  }
  
  QVariant property({{compoundname}}* selfObj, const char *name) {
    return selfObj->property(name);
  }
  
  void setProperty({{compoundname}}* selfObj, const char *name, QVariant value) {
    return selfObj->setProperty(name, value);
  }
  
};