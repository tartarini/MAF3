
#include <{{ compoundname }}.h>

{% if namespace %}using namespace {{ namespace }}; {% endif %}

// {{ compoundname }} decorator
class {{compoundname}}Decorator : public QObject
{
  Q_OBJECT

public Q_SLOTS:
  {% for method in public_func %}
    {% if not method.operator and not method.destructor %}
        {% if method.name == compoundname %}
            {{ compoundname }}* new_{{ compoundname }}({% for p in method.param %} {{p.type}} {{p.declname}}{% if not loop.last%},{%endif%}{% endfor %}) {
                return new {{ compoundname }}({% for p in method.param %} {{ p.declname }}{% if not loop.last%},{%endif%} {% endfor %});
        }{% else %}
            {% if method.type %}{{ method.type }}{%else%}void{%endif%} {{ method.name}} ( {{compoundname}}* obj {% for p in method.param %}, {{p.type}} {{p.declname}}{% endfor %}) {
                {% if method.type and method.type != 'void'%}return{% endif %} obj->{{ method.name }}({% for p in method.param %} {{ p.declname }}{% if not loop.last%},{%endif%}{% endfor %});
        }{% endif %}
    {% endif %}
  {% endfor %}
  void delete_{{compoundname}}({{compoundname}}* obj) {
    {% if not destructor_method %} delete obj;{% else %} obj->{{destructor_method}}();{%endif%}
  }
};